//
//  StateHeroAttack.cpp
//  Game
//
//  Created by fuchenhao on 2/28/15.
//
//

#include "StateHeroAttack.h"
#include "HeroSystem.h"
#include "../../AICommonHelper.h"
#include "BattleEvent.h"
#include "AudioSystem.h"

StateHeroAttack::StateHeroAttack(int m_lastStateCode):
m_lastStateCode(m_lastStateCode)
{
}

StateHeroAttack::~StateHeroAttack()
{
    
}

void StateHeroAttack::enter(entityx::Entity& entity)
{
    

}

void StateHeroAttack::exit(entityx::Entity& entity)
{
    
}

void StateHeroAttack::sendRevengeEvent(entityx::Entity& entity)
{
    int target = entity.component<BattleComponent::General>()->target;
    entityx::Entity* pTargetEntity = _BATTLE_ENTITY.getEntity(target);
    if(pTargetEntity != NULL)
    {
        BattleEvent::BeAttacked event(*pTargetEntity, entity);
        _ENTITY_EVENT.emit(event);
    }
}

int StateHeroAttack::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto animation = entity.component<BattleComponent::Animation>();
    if(m_subState == SUB_STATE::ATTACK)
    {
        if (animation->battleAnimation->isComplete())
        {
            // 每次攻击完成后寻找新的目标，随机攻击身前身后
            int targetId = _TARGET_FINDER.findHeroFrontNearbyTarget(entity);
            if(targetId > 0)
            {
                if(MapHelper::isHero(targetId))
                {
                    targetId = _TARGET_FINDER.findHeroNearbyTarget(entity);
                }
            }
            
            m_subState = SUB_STATE::APPROACH;
        }
        return m_result;
    }
    
    auto general = entity.component<BattleComponent::General>();
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(general->target);
    if (pTargetEntity == nullptr || !_HERO_MOVEMENT.canAttack(entity, *pTargetEntity))
    {
        _RETURN_STATE_RESULT(ENEMY_DEAD);
    }
    
    if (m_subState == SUB_STATE::APPROACH)
    {
        bool inAttackRange = _HERO_MOVEMENT.moveApproachAttackRange(dt, entity, *pTargetEntity);
        if (inAttackRange)
        {
            if (animation->battleAnimation->getCurrent() == BattleConfig::ANI_RUN)
            {
                ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
            }
            m_subState = SUB_STATE::COOLDOWN;
        }
        else
        {
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN, true);
        }
        return m_result;
    }

    if (m_subState == SUB_STATE::COOLDOWN)
    {
        if(isTimeToFire(entity))
        {
            sendRevengeEvent(entity);
            
            doAttack(entity);
            
            playAttackAnimation(entity, *pTargetEntity);
            
            m_subState = SUB_STATE::ATTACK;
        }
    }

    return m_result;
}

void StateHeroAttack::doAttack(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto position = entity.component<BattleComponent::Position>();
    auto general = entity.component<BattleComponent::General>();
    
    BattleConfig::AttackData attackData(identify->id, getAttackDelay());
    attackData.setTargetId(general->target);
    attackData.damagePosition.x = position->x;
    attackData.damagePosition.y = position->y;
    attackData.damagePosition.z = position->z;
    attackData.damagePosition.speedH = HERO_ATTACK_SPEED_H;
    attackData.damagePosition.speedV = HERO_ATTACK_SPEED_V;
    attackData.damagePosition.gravity = HERO_ATTACK_GRAVITY;
    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    
    std::string eventName = AUDIOEVENTS_HERO_ATTACK_MAN;
    bool isWoman = isHeroWoman(entity);
    if(isWoman)
    {
        eventName = AUDIOEVENTS_HERO_ATTACK_WOMAN;
    }
    
    std::string triggerName = "hero"; //getHeroSoldierAudioTriggerName(entity);
    BattleEvent::BattleSound soundEvent(eventName, triggerName, entity);
    _ENTITY_EVENT.emit(soundEvent);
}

void StateHeroAttack::playAttackAnimation(entityx::Entity& entity, entityx::Entity& targetEntity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();
    auto animation = entity.component<BattleComponent::Animation>();
    auto targetPos = targetEntity.component<BattleComponent::Position>();
    bool invert = false;
    
    if (targetPos->x > position->x && identify->faceTo == BattleConfig::FACE_TO_LEFT) invert = true;
    if (targetPos->x < position->x && identify->faceTo == BattleConfig::FACE_TO_RIGHT) invert = true;
    
    std::string animationName;
    if (invert)
    {
        animationName = BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_ATTACK3);
    }
    else
    {
        animationName = BattleConfig::getInstance()->getAnimationName(cocos2d::random() % 2 == 0 ? BattleConfig::ANI_ATTACK1 : BattleConfig::ANI_ATTACK2);
    }
    animation->battleAnimation->playAnimation(animationName, false);
    
    _EFFECT_CREATOR.createNormalAttackEffect(entity, animationName + "_L");
}

float StateHeroAttack::getAttackDelay()
{
    return 0.5;
}
