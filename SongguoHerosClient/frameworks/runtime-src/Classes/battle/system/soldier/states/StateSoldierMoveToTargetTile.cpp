//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierMoveToTargetTile.h"

#include "../../AICommonHelper.h"
#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "BattleEvent.h"

StateSoldierMoveToTargetTile::StateSoldierMoveToTargetTile(int m_lastStateCode):
m_lastStateCode(m_lastStateCode)
{
    
}

StateSoldierMoveToTargetTile::~StateSoldierMoveToTargetTile()
{
    
}

void StateSoldierMoveToTargetTile::enter(entityx::Entity& entity)
{
    auto target = entity.component<BattleComponent::Target>();
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    
    _SIMPLE_MOVEMENT.updateDirection(direction, position, target);
}

void StateSoldierMoveToTargetTile::exit(entityx::Entity& entity)
{
    
}

int StateSoldierMoveToTargetTile::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    ensureAnimation(entity.component<BattleComponent::Animation>()->battleAnimation, BattleConfig::ANI_RUN);
    
    if(_SOLDIER_MOVEMENT.arrivePosition(dt, entity) || _SOLDIER_MOVEMENT.move(dt, entity))
    {
        _RETURN_STATE_RESULT(ARRIVE);
    }
    
    return m_result;
}
