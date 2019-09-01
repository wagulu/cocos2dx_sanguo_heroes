//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierProtectHero.h"
#include "../../AICommonHelper.h"

StateSoldierProtectHero::StateSoldierProtectHero()
{
    //    m_result = WORKING;
    //    m_subState = SUB_STATE::NORMAL;
}

StateSoldierProtectHero::~StateSoldierProtectHero()
{
    
}

void StateSoldierProtectHero::enter(entityx::Entity& entity)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto identify = entity.component<BattleComponent::Identify>();
    soldier->target = (identify->side == BattleConfig::SIDE_LEFT) ? LEFT_HERO : RIGHT_HERO;
    
}

void StateSoldierProtectHero::exit(entityx::Entity& entity)
{
    BattleComponent::Soldier::Handle soldier = entity.component<BattleComponent::Soldier>();
    soldier->cleanTarget();
    
}

bool StateSoldierProtectHero::isArriveHero(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    
    bool arrived = false;
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(identify->side == BattleConfig::SIDE_LEFT ? LEFT_HERO : RIGHT_HERO);
    if (pTargetEntity != nullptr && _SOLDIER_MOVEMENT.canAttack(entity, *pTargetEntity))
    {
        arrived = true;
    }
    return arrived;
}


int StateSoldierProtectHero::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(soldier->target);
    if (pTargetEntity == nullptr)
    {
        _RETURN_STATE_RESULT(HERO_NOT_EXIST);
    }
    
    if(isArriveHero(entity))
    {
        _RETURN_STATE_RESULT(ARRIVE); //near the hero
        
    }
    
    BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();
    
    
    if(m_subState == SUB_STATE::NORMAL)
    {
        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
        
        {
            _SOLDIER_MOVEMENT.updateDirection(entity);
            
            if(_SOLDIER_MOVEMENT.move(dt, entity))
            {
                m_subState = SUB_STATE::TO_WIND;
            }
        }
    }
    else if(m_subState == SUB_STATE::TO_WIND)
    {
        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);

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
        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
        
        if(_SOLDIER_MOVEMENT.arrivePosition(dt, entity))
        {
            m_subState = SUB_STATE::NORMAL;
        }
        else
        {
            if(_SOLDIER_MOVEMENT.move(dt, entity))
            {
                toWaitSubState();
//                m_subState = SUB_STATE::TO_WIND;
            }
        }
    }
    else if(m_subState == SUB_STATE::WAIT)
    {
        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE);
        
        m_waitTimeLeft -= dt;
        if(m_waitTimeLeft < 0)
        {
            if(shouldEnterBowManAttack(entity))
            {
                _RETURN_STATE_RESULT(BOWMAN_ATTACK);
            }
            
            m_subState = SUB_STATE::RANDOM;
        }
        
    }
    else if(m_subState == SUB_STATE::RANDOM)
    {
        if(_PATH_FINDING.randomRun(entity))
        {
            toWaitSubState();
        }
        else
        {
            m_subState = SUB_STATE::WIND;
        }
        
    }
    
    return m_result;
    
}

void StateSoldierProtectHero::toWaitSubState()
{
    m_waitTimeLeft = 0.2;
    m_subState = SUB_STATE::WAIT;
}

//void StateSoldierProtectHero::tryToWindOrRandomRun(entityx::Entity& entity)
//{
//    if(WorldBattle::getInstance()->getMapControl()->windSoldier(entity, true))
//    {
//        if(WorldBattle::getInstance()->getMapControl()->randomRun(entity))
//        {
//            m_subState = SUB_STATE::RANDOM_RUN;
//        }
//        else
//        {
//            m_subState = SUB_STATE::WIND;
//        }
//    }
//    else
//    {
//        m_subState = SUB_STATE::WIND;
//    }
//
//}
