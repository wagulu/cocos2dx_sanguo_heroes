//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierMoveToEnemy.h"
#include "../../AICommonHelper.h"

//#define RESEARCH_TARGET_INTERVAL (1)

StateSoldierMoveToEnemy::StateSoldierMoveToEnemy()
{
}

StateSoldierMoveToEnemy::~StateSoldierMoveToEnemy()
{
    
}

void StateSoldierMoveToEnemy::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();

    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);

    _TARGET_FINDER.findTarget(entity);
}

void StateSoldierMoveToEnemy::exit(entityx::Entity& entity)
{
    
}

int StateSoldierMoveToEnemy::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    BattleComponent::Soldier::Handle soldier = entity.component<BattleComponent::Soldier>();

    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(soldier->target);
    if(pTargetEntity == nullptr)
    {
        if(isTimeToResearchTarget(entity))
        {
            _TARGET_FINDER.findTarget(entity);
        }
        m_subState = SUB_STATE::NORMAL;
        return m_result;
    }
    else if(MapHelper::isHero(pTargetEntity->component<BattleComponent::Identify>()->id))
    {
        _RETURN_STATE_RESULT(AIMING_ENEMY_HERO);
    }

    if(isTimeToFire(entity, 2, false))
    {
        if(shouldEnterBowManAttack(entity))
        {
            _RETURN_STATE_RESULT(BOWMAN_ATTACK);
        }
    }
    if(_SOLDIER_MOVEMENT.canAttack(entity, *pTargetEntity))
    {
        _RETURN_STATE_RESULT(ATTACK_ENEMY);
    }

    if(m_subState == SUB_STATE::NORMAL)
    {
        _SOLDIER_MOVEMENT.updateDirection(entity);
        
        if(_SOLDIER_MOVEMENT.move(dt, entity))
        {
            m_subState = SUB_STATE::TO_WIND;
        }
    }
    else if(m_subState == SUB_STATE::TO_WIND)
    {
        if(_PATH_FINDING.roll(entity, true))
        {
            toWaitSubState();
        }
        else
        {
            m_subState = SUB_STATE::WIND;
        }
        
    }
    else if(m_subState == SUB_STATE::WIND)
    {
        if(_SOLDIER_MOVEMENT.arrivePosition(dt, entity))
        {
            m_subState = SUB_STATE::NORMAL;
        }
        else
        {
            if(_SOLDIER_MOVEMENT.move(dt, entity))
            {
                m_subState = SUB_STATE::TO_WIND;
            }

        }
    }
    else if(m_subState == SUB_STATE::WAIT)
    {
        m_waitTimeLeft -= dt;
        if(m_waitTimeLeft < 0)
        {
            if(isTimeToResearchTarget(entity))
            {
                _TARGET_FINDER.findTarget(entity);
            }
//            researchTarget(entity);
            
            m_subState = SUB_STATE::NORMAL;
        }
    }

    return m_result;
}

void StateSoldierMoveToEnemy::toWaitSubState()
{
    m_waitTimeLeft = 1;
    m_subState = SUB_STATE::WAIT;
}

//bool StateSoldierMoveToEnemy::researchTarget(entityx::Entity& entity)
//{
//    float currentTime = _BATTLE_TIME;
//    if(currentTime - m_lastResearchTargetTime > RESEARCH_TARGET_INTERVAL)
//    {
//        m_lastResearchTargetTime = currentTime;
//        _TARGET_FINDER.findTarget(entity);
//
//        return true;
//    }
//    
//    return false;
//}

