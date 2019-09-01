//
//  ArrowMovement.cpp
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#include "ArrowMovement.h"
#include "BattleWorld.h"

ArrowMovement::~ArrowMovement()
{
    
}

ArrowMovement::ArrowMovement()
{
    
}

BattleConfig::MoveArrowResult ArrowMovement::moveLine(float dt, entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    auto arrow = entity.component<BattleComponent::SkillArrow>();
    
    int preTileX = MapHelper::getTileX(position->x);
    int preTileZ = MapHelper::getTileX(position->z);

    position->x += dt * direction->x;
    position->y += dt * direction->y;
    position->z += dt * direction->z;
    
    int currentTileX = MapHelper::getTileX(position->x);
    int currentTileZ = MapHelper::getTileZ(position->z);

    if(position->x <= 0 || position->x >= STAGE_WIDTH * GRID_WIDTH || position->z <= 0 || position->z >= STAGE_HEIGHT * GRID_HEIGHT)
    {
        return BattleConfig::ARROW_OUT;
    }
    
    if(arrow->arrowData.range > 0 && abs(currentTileX - arrow->startX) > arrow->arrowData.range)
    {
        return BattleConfig::ARROW_OUT;
    }
 
    if(currentTileX == preTileX && currentTileZ == preTileZ)
    {
        return BattleConfig::ARROW_NO_HIT;
    }
    
    bool hasTarget = false;
    arrow->targets.clear();
    arrow->hitHero = false;
    for(int z = currentTileZ - arrow->arrowData.radius; z <= currentTileZ + arrow->arrowData.radius; z++)
    {
        if(MapHelper::validTile(z, currentTileX))
        {
            int id = _BATTLE_ENTITY.getTarget(z, currentTileX);
            if(id != 0 && MapHelper::getSide(id) != arrow->arrowData.side)
            {
                hasTarget = true;
                arrow->targets.push_back(id);
                
                if(z == currentTileZ && MapHelper::getEntityType(id) == BattleConfig::ENTITY_HERO)
                {
                    arrow->hitHero = true;
                }

            }
        }
    }
    if(hasTarget) return BattleConfig::ARROW_HIT;
    
    return BattleConfig::ARROW_NO_HIT;
}

BattleConfig::MoveArrowResult ArrowMovement::moveCurve(float dt, entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    auto arrow = entity.component<BattleComponent::SkillArrow>();
    
    int preTileX = MapHelper::getTileX(position->x);
    int preTileZ = MapHelper::getTileX(position->z);
    
    position->x += dt * direction->x;
    position->y += dt * direction->y;
    //position->z += dt * direction->z;//change this one
    arrow->phase += arrow->arrowData.frequency * 360 * dt;
    if(arrow->phase > 360.0f) arrow->phase -= 360.0f;
    position->z = float(arrow->startZ * GRID_WIDTH) + arrow->arrowData.amplitude * sin(arrow->phase * 3.14159 / 180);
    
    
    int currentTileX = MapHelper::getTileX(position->x);
    int currentTileZ = MapHelper::getTileZ(position->z);
    
    if(position->x <= 0 || position->x >= STAGE_WIDTH * GRID_WIDTH || position->z <= 0 || position->z >= STAGE_HEIGHT * GRID_HEIGHT)
    {
        return BattleConfig::ARROW_OUT;
    }
    
    if(arrow->arrowData.range > 0 && abs(currentTileX - arrow->startX) > arrow->arrowData.range)
    {
        return BattleConfig::ARROW_OUT;
    }
    
    if(currentTileX == preTileX && currentTileZ == preTileZ)
    {
        return BattleConfig::ARROW_NO_HIT;
    }
    
    bool hasTarget = false;
    arrow->targets.clear();
    arrow->hitHero = false;
    for(int z = currentTileZ - arrow->arrowData.radius; z <= currentTileZ + arrow->arrowData.radius; z++)
    {
        if(MapHelper::validTile(z, currentTileX))
        {
            int id = _BATTLE_ENTITY.getTarget(z, currentTileX);
            if(id != 0 && MapHelper::getSide(id) != arrow->arrowData.side)
            {
                hasTarget = true;
                arrow->targets.push_back(id);
                
                if(z == currentTileZ && MapHelper::getEntityType(id) == BattleConfig::ENTITY_HERO)
                {
                    arrow->hitHero = true;
                }
                
            }
        }
    }
    if(hasTarget) return BattleConfig::ARROW_HIT;
    
    return BattleConfig::ARROW_NO_HIT;
}

BattleConfig::MoveArrowResult ArrowMovement::moveVertical(float dt, entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    auto arrow = entity.component<BattleComponent::SkillArrow>();
    
    int preTileZ = MapHelper::getTileX(position->z);
    
    position->x += dt * direction->x;
    position->y += dt * direction->y;
    position->z += dt * direction->z;
    
    int currentTileX = MapHelper::getTileX(position->x);
    int currentTileZ = MapHelper::getTileZ(position->z);
    
    if(position->x <= 0 || position->x >= STAGE_WIDTH * GRID_WIDTH || position->z <= 0 || position->z >= STAGE_HEIGHT * GRID_HEIGHT)
    {
        return BattleConfig::ARROW_OUT;
    }
    
    if(arrow->arrowData.range > 0 && abs(currentTileZ - arrow->startZ) > arrow->arrowData.range)
    {
        return BattleConfig::ARROW_OUT;
    }
    
    if(currentTileZ == preTileZ)
    {
        return BattleConfig::ARROW_NO_HIT;
    }
    
    bool hasTarget = false;
    arrow->targets.clear();
    arrow->hitHero = false;
    for(int x = currentTileX - arrow->arrowData.radius; x <= currentTileX + arrow->arrowData.radius; x++)
    {
        if(MapHelper::validTile(currentTileZ, x))
        {
            int id = _BATTLE_ENTITY.getTarget(currentTileZ, x);
            if(id != 0 && MapHelper::getSide(id) != arrow->arrowData.side)
            {
                hasTarget = true;
                arrow->targets.push_back(id);
                
                if(MapHelper::getEntityType(id) == BattleConfig::ENTITY_HERO)
                {
                    arrow->hitHero = true;
                }
                
            }
        }
    }
    if(hasTarget) return BattleConfig::ARROW_HIT;
    
    return BattleConfig::ARROW_NO_HIT;
}

void ArrowMovement::updateDirection(float dt, entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    auto arrow = entity.component<BattleComponent::SkillArrow>();
    
    int targetId = (arrow->arrowData.side == BattleConfig::SIDE_LEFT) ? RIGHT_HERO : LEFT_HERO;
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity != nullptr)
    {
        auto targetPosition = pTargetEntity->component<BattleComponent::Position>();
        
        cocos2d::Vec2 vec(targetPosition->x - position->x, targetPosition->z - position->z);
        vec.normalize();
        direction->x = arrow->arrowData.speed * vec.x;
        direction->z = arrow->arrowData.speed * vec.y;
    }
}



















