//
//  SoldierMovement.cpp
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#include "SoldierMovement.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "cocos2d.h"

SoldierMovement::SoldierMovement(BattleEntity* entities, BattleStage* stage)
{
    m_entities = entities;
    m_stage = stage;
}

SoldierMovement::~SoldierMovement()
{
}

void SoldierMovement::updateDirection(entityx::Entity& entity)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto* pTargetEntity = m_entities->getEntity(soldier->target);
    if (pTargetEntity == nullptr)
    {
        return;
    }
    
    auto direction = entity.component<BattleComponent::Direction>();
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();
    auto target = entity.component<BattleComponent::Target>();
    auto targetPos = pTargetEntity->component<BattleComponent::Position>();
    
    int xDirection = 0;
    if (position->x == targetPos->x) xDirection = (identify->side == BattleConfig::SIDE_LEFT ? -1 : 1);
    else xDirection = (position->x < targetPos->x ? -1 : 1);

    float targetX;
    int targetTileX = MapHelper::getTileX(targetPos->x);
    int tileXBias;
    if (MapHelper::getEntityType(soldier->target) == BattleConfig::ENTITY_HERO)
    {
        tileXBias = (xDirection > 0 ? 1 : -2);
        targetX = MapHelper::getCenterX(targetTileX + tileXBias, true);
    }
    else
    {
        tileXBias = (xDirection > 0 ? 1 : -1);
        targetX = MapHelper::getCenterX(targetTileX + tileXBias, false);
    }
    target->set(targetX, 0, targetPos->z);
    
    _SIMPLE_MOVEMENT.updateDirection(direction, position, target);
    
    identify->faceTo = (targetX > position->x ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
}

bool SoldierMovement::move(float dt, entityx::Entity& entity)
{
    static cocos2d::Vec3 m_tempCurrentPos;
    static cocos2d::Vec3 m_tempTargetPos;
    
    auto position = entity.component<BattleComponent::Position>();
    auto target = entity.component<BattleComponent::Target>();
    float speed = entity.component<BattleComponent::Property>()->get(BattleConfig::Property::SPEED);
    
    m_tempCurrentPos.set(position->x, position->y, position->z);
    m_tempTargetPos.set(target->x, target->y, target->z);
    float moveSquared = powf(speed * dt, 2);
    float distanceSquared = m_tempCurrentPos.distanceSquared(m_tempTargetPos);
    if (moveSquared >= distanceSquared)
    {
        return moveTo(target->x, target->y, target->z, entity);
    }

    cocos2d::Vec3 direction(target->x - position->x, target->y - position->y, target->z - position->z);
    direction.normalize();
//    auto direction = entity.component<BattleComponent::Direction>();
    float x = position->x + dt * direction.x * speed;
    float y = position->y + dt * direction.y * speed;
    float z = position->z + dt * direction.z * speed;
    
    return moveTo(x, y, z, entity);
}

bool SoldierMovement::move2(float dt, entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    float speed = entity.component<BattleComponent::Property>()->get(BattleConfig::Property::SPEED);
    
    float x = position->x + dt * direction->x * speed;
    float y = position->y + dt * direction->y * speed;
    float z = position->z + dt * direction->z * speed;
    
    return moveTo(x, y, z, entity);
}



bool SoldierMovement::moveTo(float x, float y, float z, entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto position = entity.component<BattleComponent::Position>();
    
    if (MapHelper::outOfBounds(x, z))
    {
        return true;
    }
    
    int m_prevTileX = MapHelper::getTileX(position->x);
    int m_prevTileZ = MapHelper::getTileZ(position->z);
    int m_currentTileX = MapHelper::getTileX(x);
    int m_currentTileZ = MapHelper::getTileZ(z);
    
    bool block = false;
    
    if (m_prevTileX != m_currentTileX || m_prevTileZ != m_currentTileZ)
    {
        int targetId = m_entities->getTarget(m_currentTileZ, m_currentTileX);
        if (targetId == 0)
        {
            m_entities->changeSoldierTarget(m_prevTileZ, m_prevTileX, m_currentTileZ, m_currentTileX, identify->id);
            
            updateMiniMapIcon(m_stage, entity, m_currentTileX, m_currentTileZ);
        }
        else
        {
            block = true;
        }
    }
    
    if (!block)
    {
        position->set(x, y, z);
        entity.component<BattleComponent::Animation>()->battleAnimation->setPosition(x, y, z);
    }
    
    return block;
}

bool SoldierMovement::arrivePosition(float dt, entityx::Entity& entity)
{
    auto direction = entity.component<BattleComponent::Direction>();
    auto position = entity.component<BattleComponent::Position>();
    auto target = entity.component<BattleComponent::Target>();
    auto property = entity.component<BattleComponent::Property>();
    auto animation = entity.component<BattleComponent::Animation>();
    
    int m_prevTileX = MapHelper::getTileX(target->x);
    int m_prevTileZ = MapHelper::getTileZ(target->z);
    int m_currentTileX = MapHelper::getTileX(position->x);
    int m_currentTileZ = MapHelper::getTileZ(position->z);
    
    //different tile
    if(m_prevTileX != m_currentTileX || m_prevTileZ != m_currentTileZ) return false;
    
    float distance = dt * property->get(BattleConfig::Property::SPEED) * 0.1;
    
    /*
    bool overPosition = false;
    if(direction->x == 0)
    {
        overPosition = (direction->z * (target->z - position->z) < 0);
    }
    else
    {
        overPosition = (direction->x * (target->x - position->x) < 0);
    }
     */
    
    bool arrived = (fabs(target->z - position->z) <= distance) && (fabs(target->x - position->x) <= distance);
    
    if(/*overPosition ||*/ arrived)
    {
        position->x = target->x;
        position->z = target->z;
        animation->battleAnimation->setPosition(position->x, position->y, position->z);
        return true;
    }
    return false;
}

bool SoldierMovement::canAttack(entityx::Entity& entity, entityx::Entity& targetEntity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto targetPos = targetEntity.component<BattleComponent::Position>();
    if (MapHelper::getTileZ(position->z) != MapHelper::getTileZ(targetPos->z))
    {
        return false;
    }
    
    auto targetIdentify = targetEntity.component<BattleComponent::Identify>();
    int tileX = MapHelper::getTileX(position->x);
    int targetTileX = MapHelper::getTileX(targetPos->x);
    if (targetIdentify->type == BattleConfig::ENTITY_SOLDIER)
    {
        return abs(tileX - targetTileX) == 1;
    }
    else
    {
        return (tileX == targetTileX + 1 || tileX == targetTileX - 2);
    }
    
    return false;
}

bool SoldierMovement::canBowmanAttack(entityx::Entity& entity, entityx::Entity& targetEntity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto targetPos = targetEntity.component<BattleComponent::Position>();
    if (MapHelper::getTileZ(position->z) != MapHelper::getTileZ(targetPos->z))
    {
        return false;
    }
    
//    auto targetIdentify = targetEntity.component<BattleComponent::Identify>();
//    int tileX = MapHelper::getTileX(position->x);
//    int targetTileX = MapHelper::getTileX(targetPos->x);
//    if (targetIdentify->type == BattleConfig::ENTITY_SOLDIER)
//    {
//        return abs(tileX - targetTileX) == 1;
//    }
//    else
//    {
//        return (tileX == targetTileX + 1 || tileX == targetTileX - 2);
//    }
    
    return true;
    
}

bool SoldierMovement::moveApproachAttackRange(float dt, entityx::Entity& entity, entityx::Entity& targetEntity)
{
    auto myIdentify = entity.component<BattleComponent::Identify>();
    auto myPos = entity.component<BattleComponent::Position>();
    auto myDir = entity.component<BattleComponent::Direction>();
    auto myTarget = entity.component<BattleComponent::Target>();
    auto targetPos = targetEntity.component<BattleComponent::Position>();
    auto targetIdentify = targetEntity.component<BattleComponent::Identify>();
    
    float x;
    float y = 0;
    float z = targetPos->z;
    
    if (targetIdentify->type == BattleConfig::ENTITY_SOLDIER)
    {
        x = targetPos->x + (myPos->x < targetPos->x ? -1 : 1) * ATTACK_DISTANCE_MELEE_MELEE;
    }
    else
    {
        if (myPos->x < targetPos->x)
        {
            x = targetPos->x - ATTACK_DISTANCE_HERO_MELEE;
            int tileX = MapHelper::getTileX(x);
            int tileXLeft = MapHelper::getTileX(targetPos->x) - 2;
            if (tileX < tileXLeft) x = tileXLeft * GRID_WIDTH;
            else if (tileX > tileXLeft) x = (tileXLeft + 1) * GRID_WIDTH - 1;
        }
        else
        {
            x = targetPos->x + ATTACK_DISTANCE_HERO_MELEE;
            int tileX = MapHelper::getTileX(x);
            int tileXRight = MapHelper::getTileX(targetPos->x) + 1;
            if (tileX < tileXRight) x = tileXRight * GRID_WIDTH;
            else if (tileX > tileXRight) x = (tileXRight + 1) * GRID_WIDTH - 1;
        }
    }
    
    myTarget->set(x, y, z);
    myIdentify->faceTo = (myPos->x < targetPos->x ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
    
     _SIMPLE_MOVEMENT.updateDirection(myDir, myPos, x, y, z);
    
    bool arrive = false;
    bool block = move(dt, entity);
    if (block || (myPos->x == x && myPos->z == z))
    {
        arrive = true;
    }
    
    return arrive;
}

bool SoldierMovement::findNearbySoldier(entityx::Entity& entity)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto identify = entity.component<BattleComponent::Identify>();
    auto position = entity.component<BattleComponent::Position>();
    
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    int targetId = m_entities->getTarget(myTileZ, myTileX + (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -1));
    
    if (targetId == 0) return false;
    if (MapHelper::getEntityType(targetId) != BattleConfig::ENTITY_SOLDIER) return false;
    
    auto pTargetEntity = m_entities->getEntity(targetId);
    if (pTargetEntity == nullptr) return false;

    auto targetIdentify = pTargetEntity->component<BattleComponent::Identify>();
    if (targetIdentify->side == identify->side) return false;
    
    soldier->target = targetIdentify->id;
    return true;
}

void SoldierMovement::knockBack(entityx::Entity& entity, float directX, float expectDist)
{
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    float x = position->x;
    if(directX > 0)
    {
        x += expectDist;
    }
    else
    {
        x -= expectDist;
    }
    float y = position->y;
    float z = position->z;
    
    int oldTileX = MapHelper::getTileX(position->x);
    int oldTileZ = MapHelper::getTileZ(position->z);
    int newTileX = MapHelper::getTileX(x);
    int newTileZ = MapHelper::getTileZ(z);
    
    bool block = false;
    if(newTileX != oldTileX || newTileZ != oldTileZ)
    {
        if(m_entities->getTarget(newTileZ, newTileX) != 0)
        {
            //assume knock back will not span MORE than 1 tile
            if(directX > 0)
            {
                x = (oldTileX + 1) * GRID_WIDTH - 1;
            }
            else
            {
                x = oldTileX * GRID_WIDTH + 1;
            }
        }
        else
        {
            m_entities->changeSoldierTarget(oldTileZ, oldTileX, newTileZ, newTileX, identify->id);
        }
    }
    else
    {

    }
    
    if(!block)
    {
        position->set(x, y, z);
        //        position->x = x;
        //        position->y = y;
        //        position->z = z;
        //        auto soldier = entity.component<BattleComponent::Soldier>();
        
        auto animation = entity.component<BattleComponent::Animation>();
        animation->battleAnimation->setPosition(x, y, z);

        updateMiniMapIcon(m_stage, entity, newTileX, newTileZ);
    }

}

bool SoldierMovement::isSoldierPositionLostInTileMap(int soldierId, float z, float x)
{
    int tileZ = MapHelper::getTileZ(z);
    int tileX = MapHelper::getTileX(x);
    if (!MapHelper::validTile(tileZ, tileX)) return false;
    int targetId = _BATTLE_ENTITY.getTarget(tileZ, tileX);
    if (targetId == 0 || targetId != soldierId) return true;
    return false;
}
