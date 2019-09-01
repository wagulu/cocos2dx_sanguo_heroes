//
//  StateSoldierArchorAttack.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 4/16/15.
//
//

#include "StateSoldierArchorAttack.h"
#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "AnchorArrowCreator.h"

StateSoldierArchorAttack::StateSoldierArchorAttack()
{
    
}

StateSoldierArchorAttack::~StateSoldierArchorAttack()
{
    
}

void StateSoldierArchorAttack::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
    
}

//void StateSoldierArchorAttack::sendRevengeEvent(entityx::Entity& entity)
//{
//    auto soldier = entity.component<BattleComponent::Soldier>();
//    
//    entityx::Entity* pTargetEntity = _BATTLE_ENTITY.getEntity(soldier->target);
//    if(pTargetEntity != NULL)
//    {
//        BattleEvent::BeAttacked event(*pTargetEntity, entity);
//        _ENTITY_EVENT.emit(event);
//    }
//}

void StateSoldierArchorAttack::exit(entityx::Entity& entity)
{
//    entity.component<BattleComponent::Soldier>()->cleanTarget();
}

int StateSoldierArchorAttack::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }

    bool targetValid = true;
    
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(soldier->target);
    if (pTargetEntity == nullptr || !_SOLDIER_MOVEMENT.canBowmanAttack(entity, *pTargetEntity))
    {
        targetValid = false;
    }

    auto animation = entity.component<BattleComponent::Animation>();

    if(m_subState == SUB_STATE::ATTACK)
    {
        if(m_fireDelay > 0)
        {
            m_fireDelay -= dt;
            if(m_fireDelay <= 0)
            {
                if(targetValid)
                {
                    doAttack(entity);
                }
            }
            
        }

        if(animation->battleAnimation->isComplete())
        {
            _RETURN_STATE_RESULT(ATTACK_DONE);
        }
    }
    
    
    if(!targetValid)
    {
        _RETURN_STATE_RESULT(ENEMY_INVALID);
        
    }
    
//    if (m_subState == SUB_STATE::APPROACH)
//    {
//        bool inAttackRange = _SOLDIER_MOVEMENT.moveApproachAttackRange(dt, entity, *pTargetEntity);
//        if (inAttackRange)
//        {
//            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
//            m_subState = SUB_STATE::COOLDOWN;
//        }
//        else
//        {
//            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN, true);
//        }
//        return m_result;
//    }
    
    if (m_subState == SUB_STATE::COOLDOWN)
    {
        auto identify = entity.component<BattleComponent::Identify>();
        auto pos = entity.component<BattleComponent::Position>();
        auto targetPos = pTargetEntity->component<BattleComponent::Position>();
        identify->faceTo = (targetPos->x > pos->x)? BattleConfig::FaceTo::FACE_TO_RIGHT: BattleConfig::FaceTo::FACE_TO_LEFT;

        if(isTimeToFire(entity))
        {
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_ATTACK1, false);
            
//            sendRevengeEvent(entity);
            m_fireDelay = getAttackDelay();
//            doAttack(entity);
            
            m_subState = SUB_STATE::ATTACK;
        }
    }
    
    return m_result;
}

void StateSoldierArchorAttack::doAttack(entityx::Entity& entity)
{
    auto pos = entity.component<BattleComponent::Position>();
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto targetEntity = _BATTLE_ENTITY.getEntity(soldier->target);
    auto targetPos = targetEntity->component<BattleComponent::Position>();

    int direction = (targetPos->x > pos->x)? 1: -1;
    _ANCHOR_ARROW_CREATOR.addArrow(entity, direction);
    
//    auto identify = entity.component<BattleComponent::Identify>();
//    auto soldier = entity.component<BattleComponent::Soldier>();
//    auto position = entity.component<BattleComponent::Position>();
//    
//    BattleConfig::AttackData attackData(identify->id, getAttackDelay());
//    attackData.setTargetId(soldier->target);
//    attackData.damagePosition.x = position->x;
//    attackData.damagePosition.y = position->y;
//    attackData.damagePosition.z = position->z;
//    attackData.damagePosition.speedH = SOLDIER_ATTACK_SPEED_H;
//    attackData.damagePosition.speedV = SOLDIER_ATTACK_SPEED_V;
//    attackData.damagePosition.gravity = SOLDIER_ATTACK_GRAVITY;
//    
//    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    
}

float StateSoldierArchorAttack::getAttackDelay()
{
    return 0.5;
}
