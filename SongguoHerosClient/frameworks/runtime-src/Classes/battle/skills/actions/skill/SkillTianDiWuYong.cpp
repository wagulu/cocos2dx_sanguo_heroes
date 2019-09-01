//
//  SkillTianDiWuYong.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/8/15.
//
//

#include "SkillTianDiWuYong.h"
#include "BattleWorld.h"
#include "ColorTween.h"

SkillTianDiWuYong::~SkillTianDiWuYong()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void SkillTianDiWuYong::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL && e.attackData.fromId == m_fromId)
    {
        //soldier//bossBuff//soldierBuff
        
        auto identify = e.targetEntity.component<BattleComponent::Identify>();
        bool reserse = identify->faceTo == BattleConfig::FACE_TO_RIGHT;
        auto animation = e.targetEntity.component<BattleComponent::Animation>();
        auto position = e.targetEntity.component<BattleComponent::Position>();
        
        animation->m_tweenQueue.pushTween(TweenQueue::TARGET::G,
                                          TweenQueue::OPERATION::IN,
                                          new SimpleTween_linear(0.7, 0.3, 1.0),
                                          0);
        
        animation->m_tweenQueue.pushTween(TweenQueue::TARGET::B,
                                          TweenQueue::OPERATION::IN,
                                          new SimpleTween_linear(0.7, 0.3, 1.0),
                                          0);
        auto time = m_skillActionData->config["animationTime"]->d;
        if(identify->type == BattleConfig::ENTITY_HERO)
        {
            BattleConfig::EffectGroup* bossBuff = m_effect->getEffect("bossBuff");
            _ENTITY_EVENT.emit<BattleEvent::AddEntityAnimation>(bossBuff, e.targetEntity, reserse, true, time);
            
            if(!m_locked)
            {
                m_locked = true;
                //try to lock skill
                _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "lockSkill");
            }
        }
        else if(identify->type == BattleConfig::ENTITY_SOLDIER)
        {
            BattleConfig::EffectGroup* soldierBuff = m_effect->getEffect("soldierBuff");
            _ENTITY_EVENT.emit<BattleEvent::AddEntityAnimation>(soldierBuff, e.targetEntity, reserse, true, time);
            
            BattleConfig::EffectGroup* group = m_effect->getEffect("soldier");
            _ENTITY_EVENT.emit<BattleEvent::AddEffect>(group, reserse, position->x, position->y, position->z, 0, 0);
        }
    }
}

void SkillTianDiWuYong::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* entity = _BATTLE_ENTITY.getEntity(m_fromId);
    if(entity)
    {
        auto identify = (*entity).component<BattleComponent::Identify>();
        int targetId = identify->side == BattleConfig::SIDE_LEFT ? RIGHT_HERO : LEFT_HERO;
        auto targetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if(targetEntity)
        {
            auto position = (*targetEntity).component<BattleComponent::Position>();
            _ENTITY_MANAGER.get(m_entityId).assign<BattleComponent::SkillTileTarget>(MapHelper::getTileX(position->x), MapHelper::getTileZ(position->z));
            _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
        }
        else
        {
            remove();
        }
    }
    else
    {
        remove();
    }
}
