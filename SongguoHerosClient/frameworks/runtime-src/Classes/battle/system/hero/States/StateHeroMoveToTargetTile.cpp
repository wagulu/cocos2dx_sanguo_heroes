//
//  StateHeroMoveToTargetTile.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/9/15.
//
//

#include "StateHeroMoveToTargetTile.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

StateHeroMoveToTargetTile::StateHeroMoveToTargetTile(const BattleConfig::Tile& targetTile)
{
    m_targetX = MapHelper::getCenterX(targetTile.tileX, true);
    m_targetY = 0;
    m_targetZ = MapHelper::getCenterZ(targetTile.tileZ);
}

StateHeroMoveToTargetTile::~StateHeroMoveToTargetTile()
{
}

void StateHeroMoveToTargetTile::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
}

void StateHeroMoveToTargetTile::exit(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto direction = entity.component<BattleComponent::Direction>();
    
    if (identify->side == BattleConfig::SIDE_LEFT)
    {
        identify->faceTo = BattleConfig::FACE_TO_RIGHT;
        direction->x = 1;
        direction->y = 0;
        direction->z = 0;
    }
    else
    {
        identify->faceTo = BattleConfig::FACE_TO_LEFT;
        direction->x = -1;
        direction->y = 0;
        direction->z = 0;
    }
}

int StateHeroMoveToTargetTile::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    auto identify = entity.component<BattleComponent::Identify>();
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    
    _SIMPLE_MOVEMENT.updateDirection(direction, position, m_targetX, m_targetY, m_targetZ);
    
    identify->faceTo = (direction->x > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
    
    _HERO_MOVEMENT.move(dt, m_targetX, m_targetY, m_targetZ, entity, HeroMovement::MoveIgnoreType::IGNORE_ALL_SOLDIER, 300);
    
    if (position->z == m_targetZ && position->x == m_targetX)
    {
        _RETURN_STATE_RESULT(ARRIVE);
    }
    
    return m_result;
}
