//
//  SkillLianHuan.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/29/15.
//
//

#include "SkillLianHuan.h"
#include "BattleWorld.h"

void SkillLianHuan::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
    
    m_radius = m_skillActionData->config["radius"]->d;
    m_duration = caclFormulaResult(*m_skillActionData->config["duration"]->s);
    m_hurtPercent = caclFormulaResult(*m_skillActionData->config["hurtPercent"]->s);
}

SkillLianHuan::~SkillLianHuan()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void SkillLianHuan::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (elapse >= m_duration)
    {
        remove();
    }
}

void SkillLianHuan::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getTargetId() == m_fromId && e.attackData.finalHpChanged < 0)
    {
        auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
        if (pFromEntity)
        {
            createHeroBeingHurtEffect(*pFromEntity);
            
            BattleConfig::TileResults results;
            _TARGET_FINDER.findDiamondAreaTilesCenterOnHero(m_radius, *pFromEntity, results);
            
            std::vector<int> targets;
            targets.push_back(e.attackData.fromId);
            int targetId;
            float delay = 0;
            auto fromPosition = pFromEntity->component<BattleComponent::Position>();
            
            for (auto* pTile : results.tiles)
            {
                targetId = _BATTLE_ENTITY.getTarget(pTile->tileZ, pTile->tileX);
                if (targetId > 0 && MapHelper::getSide(targetId) != MapHelper::getSide(m_fromId) && targetId != e.attackData.fromId)
                {
                    targets.push_back(targetId);
                }
            }
            
            for (int i = 0; i < targets.size(); i++)
            {
                targetId = targets[i];
                auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
                if (pTargetEntity)
                {
                    float targetOffsetY = (MapHelper::isHero(targetId) ? 120 : 40);
                    
                    auto targetPosition = pTargetEntity->component<BattleComponent::Position>();
                    cocos2d::Vec3 fromPos(fromPosition->x, fromPosition->y + 220, fromPosition->z);
                    cocos2d::Vec3 toPos(targetPosition->x, targetPosition->y + targetOffsetY, targetPosition->z);
                    
                    createLineEffect(fromPos, toPos, delay);
                    
                    BattleConfig::AttackData attackData(m_fromId, delay);
                    attackData.setTargetId(targetId);
                    attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
                    attackData.damageData.damageBase = abs(e.attackData.finalHpChanged) * m_hurtPercent;
                    attackData.damageData.damageStep = 0;
                    attackData.damageData.damageFactor = 0;
                    attackData.damagePosition.x = fromPosition->x;
                    attackData.damagePosition.y = 0;
                    attackData.damagePosition.z = fromPosition->z;
                    attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH;
                    attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV;
                    attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
                    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
                }
            }
        }
    }
}

void SkillLianHuan::createHeroBeingHurtEffect(entityx::Entity& entity)
{
    auto effectEntity = _ENTITY_MANAGER.create();
    auto* pEffectGroup = m_effect->getEffect("hit_back");
    bool reverse = (entity.component<BattleComponent::Identify>()->faceTo == BattleConfig::FACE_TO_RIGHT);
    
    BattleEvent::AddEffectToEntity effect(pEffectGroup, reverse, entity, 0, 0, m_fromId);
    effect.effectEntity = effectEntity;
    _ENTITY_EVENT.emit(effect);
    
    auto animationGroup = effectEntity.component<BattleComponent::AnimationGroup>();
    animationGroup->m_tweenQueue.clearTween();
    animationGroup->m_tweenQueue.pushTween(TweenQueue::TARGET::G, TweenQueue::OPERATION::IN, new SimpleTween_linear(0.0, 0.5, 0.5), 0.0);
    animationGroup->m_tweenQueue.pushTween(TweenQueue::TARGET::G, TweenQueue::OPERATION::IN, new SimpleTween_linear(0.0, 1.0, 1.0), 0.1);
    animationGroup->m_tweenQueue.pushTween(TweenQueue::TARGET::B, TweenQueue::OPERATION::IN, new SimpleTween_linear(0.0, 0.5, 0.5), 0.0);
    animationGroup->m_tweenQueue.pushTween(TweenQueue::TARGET::B, TweenQueue::OPERATION::IN, new SimpleTween_linear(0.0, 1.0, 1.0), 0.1);
}

void SkillLianHuan::createLineEffect(const cocos2d::Vec3& fromPos, const cocos2d::Vec3& toPos, float delay)
{
    auto effectEntity = _ENTITY_MANAGER.create();
    auto* pEffectGroup = (toPos.z > fromPos.z ? m_effect->getEffect("hit_line_back") : m_effect->getEffect("hit_line_front"));
    
    BattleEvent::AddEffect effect(pEffectGroup, false, fromPos.x, fromPos.y, fromPos.z, delay, 0, m_fromId);
    effect.effectEntity = effectEntity;
    _ENTITY_EVENT.emit(effect);
    
    cocos2d::Vec3 origDir(1, 0, 0);
    cocos2d::Vec3 targetDir(cocos2d::Vec3(fromPos.x, fromPos.y, -fromPos.z), cocos2d::Vec3(toPos.x, toPos.y, -toPos.z));
    cocos2d::Vec3 axis;
    cocos2d::Vec3::cross(origDir, targetDir, &axis);
    float angle = cocos2d::Vec3::angle(origDir, targetDir);
    cocos2d::Quaternion rotation(axis, angle);
    float distance = fromPos.distance(toPos);
    
//    CCLOG("pos %d, %d, %f, %f, %f, %f", MapHelper::getTileX(toPos.x), MapHelper::getTileZ(toPos.z), 180.0f / M_PI * angle, axis.x, axis.y, axis.z);
    
    auto animationGroup = effectEntity.component<BattleComponent::AnimationGroup>();
    auto& animations = *animationGroup->battleAnimation->getAnimations();
    for (auto* pAni : animations)
    {
        pAni->getAnimation()->setScaleX(CHARACTER_SCALE * distance / 95);
        pAni->getAnimation()->setRotationQuat(rotation);
    }
}

