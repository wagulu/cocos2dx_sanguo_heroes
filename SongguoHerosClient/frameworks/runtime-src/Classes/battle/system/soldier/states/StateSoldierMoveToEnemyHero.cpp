//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierMoveToEnemyHero.h"
#include "../../AICommonHelper.h"
#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "BattleEvent.h"

StateSoldierMoveToEnemyHero::StateSoldierMoveToEnemyHero()
{
    
}

StateSoldierMoveToEnemyHero::~StateSoldierMoveToEnemyHero()
{
    
}

void StateSoldierMoveToEnemyHero::enter(entityx::Entity& entity)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto identify = entity.component<BattleComponent::Identify>();
    BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();

    //TODO, lrui, multi general
    soldier->target = (identify->side == BattleConfig::SIDE_LEFT) ? RIGHT_HERO : LEFT_HERO;
    
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
}

void StateSoldierMoveToEnemyHero::exit(entityx::Entity& entity)
{
    
}

int StateSoldierMoveToEnemyHero::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    BattleComponent::Soldier::Handle soldier = entity.component<BattleComponent::Soldier>();
//    BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();
    
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

    if(isTimeToResearchTarget(entity))
    {
        if(_SOLDIER_MOVEMENT.findNearbySoldier(entity))
        {
            _RETURN_STATE_RESULT(FIND_NEARBY_ENEMY_SOLDIER);
        }
        
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
            m_subState = SUB_STATE::RANDOM;
        }
        else
        {
            m_subState = SUB_STATE::WIND;
        }
        
    }
    else if(m_subState == SUB_STATE::WIND)
    {
//        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
        
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
    else if(m_subState == SUB_STATE::RANDOM)
    {
//        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE);
        
        if(_PATH_FINDING.randomRun(entity))
        {
            toWaitSubState();
        }
        else
        {
            m_subState = SUB_STATE::WIND;
        }
    }
    else if(m_subState == SUB_STATE::WAIT)
    {
        m_waitTimeLeft -= dt;
        if(m_waitTimeLeft < 0)
        {
            m_subState = SUB_STATE::NORMAL;
        }
    }

    return m_result;
}

void StateSoldierMoveToEnemyHero::toWaitSubState()
{
    m_waitTimeLeft = 1;
    m_subState = SUB_STATE::WAIT;
}

