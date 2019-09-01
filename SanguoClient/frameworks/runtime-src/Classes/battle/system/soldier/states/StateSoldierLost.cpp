//
//  StateSoldierLost.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/1/15.
//
//

#include "StateSoldierLost.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

StateSoldierLost::StateSoldierLost()
{
    
}

StateSoldierLost::~StateSoldierLost()
{
    
}

void StateSoldierLost::enter(entityx::Entity& entity)
{
    auto ai = entity.component<BattleComponent::AI>();
    ai->isTileLosted = true;
    
    m_block = true;
    m_randomRunDelay = 0;
    m_tileTargetLastX = -1;
    m_tileTargetLastZ = -1;
}

void StateSoldierLost::exit(entityx::Entity& entity)
{
    auto ai = entity.component<BattleComponent::AI>();
    ai->isTileLosted = false;
    
}

int StateSoldierLost::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();
    int currentTileZ = MapHelper::getTileZ(position->z);
    int currentTileX = MapHelper::getTileX(position->x);
    int targetId = _BATTLE_ENTITY.getTarget(currentTileZ, currentTileX);
    if (targetId == identify->id)
    {
        _RETURN_STATE_RESULT(NOT_LOST);
    }
    else if (targetId == 0)
    {
        _BATTLE_ENTITY.setTarget(currentTileZ, currentTileX, identify->id, targetId);
        _RETURN_STATE_RESULT(RESTORE);
    }
    
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity != nullptr)
    {
        auto targetPos = pTargetEntity->component<BattleComponent::Position>();
        float oldX = m_tileTargetLastX;
        float oldZ = m_tileTargetLastZ;
        m_tileTargetLastX = targetPos->x;
        m_tileTargetLastZ = targetPos->z;
        // wait target entity move out of the current tile, current soldier no need to leave
        if (oldX != m_tileTargetLastX || oldZ != m_tileTargetLastZ)
        {
            return m_result;
        }
    }
    
    m_randomRunDelay -= dt;
    if (m_randomRunDelay < 0) m_randomRunDelay = 0;
    
    if (m_block && m_randomRunDelay == 0)
    {
        m_block = _PATH_FINDING.randomRun(entity);
        m_randomRunDelay = 1.0;
    }
    
    if (!m_block)
    {
        ensureAnimation(entity.component<BattleComponent::Animation>()->battleAnimation, BattleConfig::ANI_RUN);
        
        m_block = _SOLDIER_MOVEMENT.move(dt, entity);
        if (!m_block)
        {
            int newTileZ = MapHelper::getTileZ(position->z);
            int newTileX = MapHelper::getTileX(position->x);
            if (newTileZ != currentTileZ || newTileX != currentTileX)
            {
                _RETURN_STATE_RESULT(MOVED_TO_NEW_TILE);
            }
        }
    }
    else
    {
        ensureAnimation(entity.component<BattleComponent::Animation>()->battleAnimation, BattleConfig::ANI_IDLE);
    }
    
    return m_result;
}
