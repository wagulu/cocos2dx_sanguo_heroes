//
//  StateHeroMoveToHeroBack.cpp
//  Game
//
//  Created by fuchenhao on 3/26/15.
//
//

#include "StateHeroMoveToHeroBack.h"
#include "../../AICommonHelper.h"

StateHeroMoveToHeroBack::StateHeroMoveToHeroBack()
{
    
}

StateHeroMoveToHeroBack::~StateHeroMoveToHeroBack()
{
    
}

void StateHeroMoveToHeroBack::enter(entityx::Entity& entity)
{
    if (!entity.has_component<BattleComponent::TargetEntity>())
    {
        m_result = TARGET_HERO_DEAD;
    }
}

void StateHeroMoveToHeroBack::exit(entityx::Entity& entity)
{
    
}

int StateHeroMoveToHeroBack::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto& targetEntity = entity.component<BattleComponent::TargetEntity>()->targetEntity;
    if (!targetEntity.valid())
    {
        _RETURN_STATE_RESULT(TARGET_HERO_DEAD);
    }
    
    auto myIdentity = entity.component<BattleComponent::Identify>();
    auto myPos = entity.component<BattleComponent::Position>();
    auto myDir = entity.component<BattleComponent::Direction>();
    auto targetPos = targetEntity.component<BattleComponent::Position>();
    auto targetIdentify = targetEntity.component<BattleComponent::Identify>();
    float tileXBias = (targetIdentify->faceTo == BattleConfig::FACE_TO_RIGHT ? -2 : 2);
    float targetX = MapHelper::getCenterX(MapHelper::getTileX(targetPos->x) + tileXBias, true);
    float targetY = targetPos->y;
    float targetZ;
    
    if (targetIdentify->faceTo == BattleConfig::FACE_TO_RIGHT && myPos->x > targetX)
    {
        targetZ = myPos->z;
        _SIMPLE_MOVEMENT.updateDirection(myDir, myPos, targetX, targetY, targetZ);
        myIdentity->faceTo = (myDir->x > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
    }
    else if (targetIdentify->faceTo == BattleConfig::FACE_TO_LEFT && myPos->x < targetX)
    {
        targetZ = myPos->z;
        _SIMPLE_MOVEMENT.updateDirection(myDir, myPos, targetX, targetY, targetZ);
        myIdentity->faceTo = (myDir->x > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
    }
    else
    {
        targetZ = targetPos->z;
        _SIMPLE_MOVEMENT.updateDirection(myDir, myPos, targetX, targetY, targetZ);
        myIdentity->faceTo = (myPos->x < targetPos->x ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
    }
    
    bool block = _HERO_MOVEMENT.move(dt, targetX, targetY, targetZ, entity);
    
    if (targetPos->z == myPos->z)
    {
        _SIMPLE_MOVEMENT.updateDirection(myDir, myPos, targetPos->x, targetPos->y, targetPos->z);
        _RETURN_STATE_RESULT(ARRIVE);
    }
    else if (block && _TARGET_FINDER.findHeroBlockMoveTarget(entity) > 0)
    {
        _RETURN_STATE_RESULT(ATTACK_ENEMY);
    }
    
    return m_result;
}
