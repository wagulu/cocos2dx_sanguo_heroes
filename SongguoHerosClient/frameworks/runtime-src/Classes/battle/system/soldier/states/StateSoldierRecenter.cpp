//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierRecenter.h"
#include "../../AICommonHelper.h"
#include "SoldierSystem.h"
#include "../../../BattleWorld.h"

StateSoldierRecenter::StateSoldierRecenter()
{
//    m_result = WORKING;
//    m_subState = SUB_STATE::NORMAL;
    
}

StateSoldierRecenter::~StateSoldierRecenter()
{
    
}

void StateSoldierRecenter::enter(entityx::Entity& entity)
{
    BattleComponent::Direction::Handle direction = entity.component<BattleComponent::Direction>();
    BattleComponent::Position::Handle position = entity.component<BattleComponent::Position>();

    int tileZ = MapHelper::getTileZ(position->z);
    int zDirection = (tileZ < STAGE_MIDDLE_Z) ? 1 : -1;
    direction->x = 0;
    direction->z = zDirection;//TODO, lrui, orderspeed

}

void StateSoldierRecenter::exit(entityx::Entity& entity)
{
    
}

bool StateSoldierRecenter::isArriveCenter(entityx::Entity& entity)
{
    BattleComponent::Position::Handle position = entity.component<BattleComponent::Position>();
    return _PATH_FINDING.arriveMiddle(position);

}


int StateSoldierRecenter::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }

    if(isArriveCenter(entity))
    {
        _RETURN_STATE_RESULT(ARRIVE);
    }

    BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();
    
//    if(m_subState == SUB_STATE::NORMAL)
//    {
//        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
//        
//        if(_SOLDIER_MOVEMENT.move(dt, entity))
//        {
//            m_subState = SUB_STATE::THINK;
//        }
//    }
//    else
        if(m_subState == SUB_STATE::WIND)
    {
        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
        
        if(_SOLDIER_MOVEMENT.arrivePosition(dt, entity) || _SOLDIER_MOVEMENT.move(dt, entity))
        {
            m_subState = SUB_STATE::THINK;
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
            
            m_subState = SUB_STATE::THINK;
        }
    }
    else if(m_subState == SUB_STATE::THINK)
    {
        BattleConfig::MoveSoldierResult state = _PATH_FINDING.moveCenter(entity);
        
        if(state == BattleConfig::MOVE_SOLDIER_ARRIVE)
        {
            toWaitSubState();
//            m_subState = SUB_STATE::WAIT;
        }
        else if(state == BattleConfig::MOVE_SOLDIER_BLOCK)
        {
            toWaitSubState();
//            m_subState = SUB_STATE::WAIT;
        }
        else //wind
        {
            m_subState = SUB_STATE::WIND;
        }
        
    }

    
    return m_result;
}

void StateSoldierRecenter::toWaitSubState()
{
    m_waitTimeLeft = 0.2;
    m_subState = SUB_STATE::WAIT;
}
