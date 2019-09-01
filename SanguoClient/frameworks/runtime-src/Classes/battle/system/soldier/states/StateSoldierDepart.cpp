//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierDepart.h"
#include "../../AICommonHelper.h"
#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "BattleEvent.h"

StateSoldierDepart::StateSoldierDepart()
{

}

StateSoldierDepart::~StateSoldierDepart()
{
    
}

void StateSoldierDepart::enter(entityx::Entity& entity)
{
    BattleComponent::Direction::Handle direction = entity.component<BattleComponent::Direction>();
    BattleComponent::Position::Handle position = entity.component<BattleComponent::Position>();

    int tileZ = MapHelper::getTileZ(position->z);
    int zDirection = (tileZ < STAGE_MIDDLE_Z) ? -1 : 1;
    direction->x = 0;
    direction->z = zDirection * 210;//TODO, lrui, orderspeed
}

void StateSoldierDepart::exit(entityx::Entity& entity)
{
    
}

int StateSoldierDepart::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto animation = entity.component<BattleComponent::Animation>();
//    BattleComponent::Property::Handle property = entity.component<BattleComponent::Property>();
    
//    entityx::Entity* pTarget = _ENTITY_FINDER.getEntity(soldier->target);
//    if(pTarget == NULL
//       || !WorldBattle::getInstance()->getMapControl()->canSoldierAttack(soldier, position)
//       )
//    {
//        _MAP_CONTROL.findTarget(soldier->id);
//        _RETURN_STATE_RESULT(ENEMY_INVALID);
//    }

    
    
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
        BattleConfig::MoveSoldierResult state = _PATH_FINDING.moveDepart(entity);
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


void StateSoldierDepart::toWaitSubState()
{
    m_waitTimeLeft = 0.2;
    m_subState = SUB_STATE::WAIT;
}
