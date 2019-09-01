//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierAttackEnemy.h"
#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "AudioSystem.h"

StateSoldierAttackEnemy::StateSoldierAttackEnemy()
{
    
}

StateSoldierAttackEnemy::~StateSoldierAttackEnemy()
{
    
}

void StateSoldierAttackEnemy::enter(entityx::Entity& entity)
{
    
}

void StateSoldierAttackEnemy::sendRevengeEvent(entityx::Entity& entity)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    
    entityx::Entity* pTargetEntity = _BATTLE_ENTITY.getEntity(soldier->target);
    if(pTargetEntity != NULL)
    {
        BattleEvent::BeAttacked event(*pTargetEntity, entity);
        _ENTITY_EVENT.emit(event);
    }
}

void StateSoldierAttackEnemy::exit(entityx::Entity& entity)
{
    entity.component<BattleComponent::Soldier>()->cleanTarget();
}

int StateSoldierAttackEnemy::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto animation = entity.component<BattleComponent::Animation>();
    if(m_subState == SUB_STATE::ATTACK)
    {
        if(animation->battleAnimation->isComplete())
        {
            m_subState = SUB_STATE::APPROACH;
        }
        return m_result;
    }
    
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(soldier->target);
    if (pTargetEntity == nullptr || !_SOLDIER_MOVEMENT.canAttack(entity, *pTargetEntity))
    {
        _RETURN_STATE_RESULT(ENEMY_INVALID);
    }

    if (m_subState == SUB_STATE::APPROACH)
    {
        bool inAttackRange = _SOLDIER_MOVEMENT.moveApproachAttackRange(dt, entity, *pTargetEntity);
        if (inAttackRange)
        {
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
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
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_ATTACK1, false);
            
            sendRevengeEvent(entity);

            doAttack(entity);
            
            m_subState = SUB_STATE::ATTACK;
        }
    }

    return m_result;
}

void StateSoldierAttackEnemy::doAttack(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto position = entity.component<BattleComponent::Position>();
    
    BattleConfig::AttackData attackData(identify->id, getAttackDelay());
    attackData.setTargetId(soldier->target);
    attackData.damagePosition.x = position->x;
    attackData.damagePosition.y = position->y;
    attackData.damagePosition.z = position->z;
    attackData.damagePosition.speedH = SOLDIER_ATTACK_SPEED_H;
    attackData.damagePosition.speedV = SOLDIER_ATTACK_SPEED_V;
    attackData.damagePosition.gravity = SOLDIER_ATTACK_GRAVITY;
    
    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    
    
//    std::string eventName = AUDIO_EVENTS_SOLDIER_PHYSICAL_ATTACK;
//    std::string triggerName = getHeroSoldierAudioTriggerName(entity);
//    BattleEvent::BattleSound soundEvent(eventName, triggerName);
//    _ENTITY_EVENT.emit(soundEvent);

}

float StateSoldierAttackEnemy::getAttackDelay()
{
    return 0.5;
}
