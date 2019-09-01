//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierMoveBack.h"

#include "../../AICommonHelper.h"


StateSoldierMoveBack::StateSoldierMoveBack()
{
}

StateSoldierMoveBack::~StateSoldierMoveBack()
{
    
}

void StateSoldierMoveBack::enter(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto direction = entity.component<BattleComponent::Direction>();
    
    if (identify->side == BattleConfig::SIDE_LEFT)
    {
        direction->x = -1;
        direction->z = 0;
        identify->faceTo = BattleConfig::FACE_TO_LEFT;
    }
    else
    {
        direction->x = 1;
        direction->z = 0;
        identify->faceTo = BattleConfig::FACE_TO_RIGHT;
    }
}

void StateSoldierMoveBack::exit(entityx::Entity& entity)
{
    
}

int StateSoldierMoveBack::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
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
        BattleConfig::MoveSoldierResult state = _PATH_FINDING.moveBack(entity);
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



void StateSoldierMoveBack::toWaitSubState()
{
    m_waitTimeLeft = 0.2;
    m_subState = SUB_STATE::WAIT;
}
