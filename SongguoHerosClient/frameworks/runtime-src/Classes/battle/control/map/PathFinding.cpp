//
//  PathFinding.cpp
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#include "PathFinding.h"

PathFinding::PathFinding(BattleEntity* entities)
{
    m_entities = entities;
}

PathFinding::~PathFinding()
{
    
}

bool PathFinding::randomRun(entityx::Entity& entity)
{
    auto direction = entity.component<BattleComponent::Direction>();
    auto position = entity.component<BattleComponent::Position>();
    auto target = entity.component<BattleComponent::Target>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    //      2
    //   1     3
    //      4
    std::vector<int> sequence;
    
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    
    if(myTileX - 1 > -1 && m_entities->getTarget(myTileZ, myTileX - 1) == 0) sequence.push_back(1);
    if(myTileZ + 1 < STAGE_HEIGHT && m_entities->getTarget(myTileZ + 1, myTileX) == 0) sequence.push_back(2);
    if(myTileX + 1 < STAGE_WIDTH && m_entities->getTarget(myTileZ, myTileX + 1) == 0) sequence.push_back(3);
    if(myTileZ - 1 > -1 && m_entities->getTarget(myTileZ - 1, myTileX) == 0) sequence.push_back(4);
    
    int moveType;
    if(sequence.size() > 0)
    {
        target->x = position->x;
        target->y = position->y;
        target->z = position->z;
        
        direction->x = 0;
        direction->y = 0;
        direction->z = 0;
        
        moveType = sequence[cocos2d::random() % sequence.size()];
        switch (moveType)
        {
            case 1:
                target->x = MapHelper::getCenterX(myTileX - 1, false);
                direction->x = -1;
                identify->faceTo = BattleConfig::FACE_TO_LEFT;
                break;
                
            case 2:
                target->z = MapHelper::getCenterZ(myTileZ + 1);
                direction->z = 1;
                break;
                
            case 3:
                target->x = MapHelper::getCenterX(myTileX + 1, false);
                direction->x = 1;
                identify->faceTo = BattleConfig::FACE_TO_RIGHT;
                break;
                
            case 4:
                target->z = MapHelper::getCenterZ(myTileZ - 1);
                direction->z = -1;
                break;
                
            default:
                break;
        }
        return false;
    }
    
    return true;
}

PathFinding::TargetTile PathFinding::pickupTargetTile(entityx::Entity& entity, bool force)
{
    TargetTile targetTile;
    
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();

    auto* pTargetEntity = m_entities->getEntity(soldier->target);
    if (pTargetEntity == nullptr)
    {
        return targetTile;
    }
    
    auto targetPos = pTargetEntity->component<BattleComponent::Position>();
    int targetTileX = MapHelper::getTileX(targetPos->x);
    int targetTileZ = MapHelper::getTileZ(targetPos->z);
    
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);

    bool targetIsHero = false;
    int leftBias = 0;
    int rightBias = 0;
    if(MapHelper::getEntityType(soldier->target) == BattleConfig::ENTITY_HERO)
    {
        targetIsHero = true;
        leftBias = -1;
        rightBias = 0;
    }
    /*
    else
    {
        if(targetPos->x > position->x)
        {
            if(targetTileX - 1 + leftBias >= 0 && m_entities->getTarget(targetTileZ, targetTileX - 1 + leftBias) == 0)
            {
                targetTiles.push_back(PathFinding::TargetTile(targetTileX - 1 + leftBias, targetTileZ));
            }
            else if(targetTileX + 1 + rightBias < STAGE_WIDTH && m_entities->getTarget(targetTileZ, targetTileX + 1 + rightBias) == 0)
            {
                targetTiles.push_back(PathFinding::TargetTile(targetTileX + 1 + rightBias, targetTileZ));
            }
        }
        else
        {
            if(targetTileX + 1 + rightBias < STAGE_WIDTH && m_entities->getTarget(targetTileZ, targetTileX + 1 + rightBias) == 0)
            {
                targetTiles.push_back(PathFinding::TargetTile(targetTileX + 1 + rightBias, targetTileZ));
            }
            else if(targetTileX - 1 + leftBias >= 0 && m_entities->getTarget(targetTileZ, targetTileX - 1 + leftBias) == 0)
            {
                targetTiles.push_back(PathFinding::TargetTile(targetTileX - 1 + leftBias, targetTileZ));
            }
        }

    }
     */
    
    //detect tile in front of target
    if(targetPos->x > position->x)
    {
        if(targetTileX - 1 + leftBias >= 0 && m_entities->getTarget(targetTileZ, targetTileX - 1 + leftBias) == 0)
        {
            targetTile.set(targetTileX - 1 + leftBias, targetTileZ);
        }
    }
    else
    {
        if(targetTileX + 1 + rightBias < STAGE_WIDTH && m_entities->getTarget(targetTileZ, targetTileX + 1 + rightBias) == 0)
        {
            targetTile.set(targetTileX + 1 + rightBias, targetTileZ);
        }
    }
    
    //detect tile beside target
    if(!targetTile.valid() && targetIsHero)
    {
        bool nearEnoughToMoveToAside = fabs(targetPos->x - position->x) < 5;
        if(nearEnoughToMoveToAside)
        {
            int startTileX = (identify->side == BattleConfig::Side::SIDE_LEFT)? (myTileX + 1): (myTileX - 1);
            int endTileX = (identify->side == BattleConfig::Side::SIDE_LEFT)? (targetTileX + 1 + rightBias): (targetTileX - 1 + leftBias);
            int stepTileX = (identify->side == BattleConfig::Side::SIDE_LEFT)? 1: -1;

            for(int x = startTileX; x != endTileX + stepTileX; x += stepTileX)
            {
                if(x >= 0 && x < STAGE_WIDTH && m_entities->getTarget(myTileZ, x) == 0)
                {
                    targetTile.set(x, myTileZ);
                    break;
                }
            }
            
        }
        
        /*
        for(int i = 1; i <= besideDist; ++i)
        {
            if(targetTileZ - i >= 0 && m_entities->getTarget(targetTileZ - i, detectTileX) == 0)
            {
                targetTiles.push_back(PathFinding::TargetTile(detectTileX, targetTileZ - i));
                break;
            }
            else if(targetTileZ + i < STAGE_HEIGHT && m_entities->getTarget(targetTileZ + i, detectTileX) == 0)
            {
                targetTiles.push_back(PathFinding::TargetTile(detectTileX, targetTileZ + i));
                break;
            }
        }
         */
        
    }
    
    //detect tile behind target
    if(!targetTile.valid())
    {
        if(targetPos->x > position->x)
        {
            if(targetTileX + 1 + rightBias < STAGE_WIDTH && m_entities->getTarget(targetTileZ, targetTileX + 1 + rightBias) == 0)
            {
                targetTile.set(targetTileX + 1 + rightBias, targetTileZ);
            }
        }
        else
        {
            if(targetTileX - 1 + leftBias >= 0 && m_entities->getTarget(targetTileZ, targetTileX - 1 + leftBias) == 0)
            {
                targetTile.set(targetTileX - 1 + leftBias, targetTileZ);
            }
        }
        
    }

    
//    int targetType = BattleConfig::POSITION_NONE;
    
    if(force && !targetTile.valid())
    {
        if(cocos2d::random() % 2 == 0)
        {
            targetTile.set(targetTileX - 1 + leftBias, targetTileZ);
        }
        else
        {
            targetTile.set(targetTileX + 1 + rightBias, targetTileZ);
        }
//        targetType = (cocos2d::random() % 2 == 0) ? BattleConfig::POSITION_LEFT : BattleConfig::POSITION_RIGHT;
    }
    
    return targetTile;
}

bool PathFinding::roll(entityx::Entity& entity, bool force)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto position = entity.component<BattleComponent::Position>();
    auto target = entity.component<BattleComponent::Target>();
    auto direction = entity.component<BattleComponent::Direction>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    auto* pTargetEntity = m_entities->getEntity(soldier->target);
    if (pTargetEntity == nullptr)
    {
        return true;
    }
    auto targetPos = pTargetEntity->component<BattleComponent::Position>();
    
    bool nearEnoughToFloating = true;//(fabs(targetPos->x - position->x) < 10);

    PathFinding::TargetTile targetTile = pickupTargetTile(entity, force);
    if(!targetTile.valid())
    {
        return true;
    }

    /*
    int targetTileX = MapHelper::getTileX(targetPos->x);
    int targetTileZ = MapHelper::getTileZ(targetPos->z);
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    
    direction->set(0, 0, 0);
    
    int leftBias = 0;
    int rightBias = 0;
    if(MapHelper::getEntityType(soldier->target) == BattleConfig::ENTITY_HERO)
    {
        leftBias = -1;
        rightBias = 1;
    }
    
    int targetType = BattleConfig::POSITION_NONE;
    if(targetPos->x > position->x)
    {
        if(targetTileX + leftBias > 0 && m_entities->getTarget(targetTileZ, targetTileX - 1 + leftBias) == 0)
        {
            targetType = BattleConfig::POSITION_LEFT;
        }
        else if(targetTileX + rightBias < STAGE_WIDTH - 1 && m_entities->getTarget(targetTileZ, targetTileX + 1 + rightBias) == 0)
        {
            targetType = BattleConfig::POSITION_RIGHT;
        }
    }
    else
    {
        if(targetTileX + rightBias < STAGE_WIDTH - 1 && m_entities->getTarget(targetTileZ, targetTileX + 1 + rightBias) == 0)
        {
            targetType = BattleConfig::POSITION_RIGHT;
        }
        else if(targetTileX + leftBias > 0 && m_entities->getTarget(targetTileZ, targetTileX - 1 + leftBias) == 0)
        {
            targetType = BattleConfig::POSITION_LEFT;
        }
    }
    
    if(force && targetType == BattleConfig::POSITION_NONE)
    {
        targetType = (cocos2d::random() % 2 == 0) ? BattleConfig::POSITION_LEFT : BattleConfig::POSITION_RIGHT;
    }
     */
    
//    if(targetType != BattleConfig::POSITION_NONE)
    {

        /*
        if(targetType == BattleConfig::POSITION_LEFT)
        {
            targetTileX = MapHelper::getTileX(targetPos->x) - 1 + leftBias;
        }
        else
        {
            targetTileX = MapHelper::getTileX(targetPos->x) + 1 + rightBias;
        }
         */
        
        bool finded = false;

        int myTileX = MapHelper::getTileX(position->x);
        int myTileZ = MapHelper::getTileZ(position->z);

        int moveX = targetTile.x - myTileX;
        int moveZ = targetTile.z - myTileZ;
        
        if(moveX != 0)
        {
            moveX = (moveX < 0) ? -1 : 1;
            if(myTileX + moveX > -1 && myTileX + moveX < STAGE_WIDTH && m_entities->getTarget(myTileZ, myTileX + moveX) == 0)
            {
                finded = true;
                moveZ = 0;
            }
        }
        
        if(moveZ != 0 && nearEnoughToFloating)
        {
            moveZ = (moveZ < 0) ? -1 : 1;
            if(myTileZ + moveZ > -1 && myTileZ + moveZ < STAGE_HEIGHT && m_entities->getTarget(myTileZ + moveZ, myTileX) == 0)
            {
                finded = true;
                moveX = 0;
            }
        }
        
        if(!finded)
        {
            if(moveX == 0)
            {
                if(identify->side == BattleConfig::Side::SIDE_LEFT)
                {
                    if(myTileX + 1 < STAGE_WIDTH && m_entities->getTarget(myTileZ, myTileX + 1) == 0)
                    {
                        finded = true;
                        moveZ = 0;
                        moveX = 1;
                    }
                    else if(myTileX - 1 >= 0 && m_entities->getTarget(myTileZ, myTileX - 1) == 0)
                    {
                        finded = true;
                        moveZ = 0;
                        moveX = -1;
                    }
                    
                }
                else
                {
                    if(myTileX - 1 >= 0 && m_entities->getTarget(myTileZ, myTileX - 1) == 0)
                    {
                        finded = true;
                        moveZ = 0;
                        moveX = -1;
                    }
                    else if(myTileX + 1 < STAGE_WIDTH && m_entities->getTarget(myTileZ, myTileX + 1) == 0)
                    {
                        finded = true;
                        moveZ = 0;
                        moveX = 1;
                    }
                    
                }
            }
        }
        
        if(!finded && nearEnoughToFloating)
        {
            if(moveZ == 0)
            {
                if(myTileZ > STAGE_MIDDLE_Z)
                {
                    if(myTileZ + 1 < STAGE_HEIGHT && m_entities->getTarget(myTileZ + 1, myTileX) == 0)
                    {
                        finded = true;
                        moveZ = 1;
                        moveX = 0;
                    }
                    else if(myTileZ - 1 >= 0 && m_entities->getTarget(myTileZ - 1, myTileX) == 0)
                    {
                        finded = true;
                        moveZ = -1;
                        moveX = 0;
                    }
                    
                }
                else
                {
                    if(myTileZ - 1 >= 0 && m_entities->getTarget(myTileZ - 1, myTileX) == 0)
                    {
                        finded = true;
                        moveZ = -1;
                        moveX = 0;
                    }
                    else if(myTileZ + 1 < STAGE_HEIGHT && m_entities->getTarget(myTileZ + 1, myTileX) == 0)
                    {
                        finded = true;
                        moveZ = 1;
                        moveX = 0;
                    }
                    
                }
            }
        }
        
        if(finded)
        {
            /*
            if(moveZ == 0)
            {
                target->z = MapHelper::getCenterZ(MapHelper::getTileZ(position->z));
                target->x = MapHelper::getCenterX(myTileX + moveX, false);
            }
            else
            {
                target->x = MapHelper::getCenterX(MapHelper::getTileX(position->x), false);
                target->z = MapHelper::getCenterZ(myTileZ + moveZ);
            }
             */
            
            target->x = MapHelper::getCenterX(myTileX + moveX, false);
            target->z = MapHelper::getCenterZ(myTileZ + moveZ);

            
            direction->x = moveX;
            direction->z = moveZ;

            if (moveX != 0)
            {
                identify->faceTo = (moveX > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
            }
            else
            {
                identify->faceTo = (position->x < targetPos->x ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
            }
            
            return false;
        }
        else
        {
            //try to random run? has target but block
            //                printf("block wind failed \n ");
            return true; //blocked
        }
    }
    
    // no target
    return true; //blocked
}

enum BattleConfig::MoveSoldierResult PathFinding::moveDepart(entityx::Entity& entity)
{
    auto direction = entity.component<BattleComponent::Direction>();
    auto position = entity.component<BattleComponent::Position>();
    auto target = entity.component<BattleComponent::Target>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    
    target->x = position->x;
    target->y = position->y;
    target->z = position->z;
    
    direction->x = 0;
    direction->y = 0;
    direction->z = 0;
    
    if(myTileZ < STAGE_MIDDLE_Z)
    {
        //down 0-9
        if(myTileZ > 0 && m_entities->getTarget(myTileZ - 1, myTileX) == 0)
        {
            target->z = MapHelper::getCenterZ(myTileZ - 1);
            direction->z = -1;
            return BattleConfig::MOVE_SOLDIER_NEXT;
        }
        else
        {
            //down 0-9 // 0-4 ok
            if(myTileZ < STAGE_DOWN_Z) return BattleConfig::MOVE_SOLDIER_ARRIVE;
            
            int xDirection = (identify->side == BattleConfig::SIDE_LEFT) ? 1 : -1;
            if(myTileX + xDirection < STAGE_WIDTH && myTileX + xDirection > -1 && m_entities->getTarget(myTileZ, myTileX + xDirection) == 0)
            {
                target->x = MapHelper::getCenterX(myTileX + xDirection, false);
                direction->x = xDirection;
                identify->faceTo = (xDirection > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
                
                return BattleConfig::MOVE_SOLDIER_NEXT;
            }
            else
            {
                return BattleConfig::MOVE_SOLDIER_BLOCK;
            }
        }
    }
    else
    {
        
        //up 10-20
        if(myTileZ + 1 < STAGE_HEIGHT && m_entities->getTarget(myTileZ + 1, myTileX) == 0)
        {
            target->z = MapHelper::getCenterZ(myTileZ + 1);
            direction->z = 1;
            return BattleConfig::MOVE_SOLDIER_NEXT;
        }
        else
        {
            //up 10-19 // 16-19
            if(myTileZ >= STAGE_UP_Z) return BattleConfig::MOVE_SOLDIER_ARRIVE;
            
            int xDirection = (identify->side == BattleConfig::SIDE_LEFT) ? 1 : -1;
            if(myTileX + xDirection < STAGE_WIDTH && myTileX + xDirection > -1 && m_entities->getTarget(myTileZ, myTileX + xDirection) == 0)
            {
                target->x = MapHelper::getCenterX(myTileX + xDirection, false);
                direction->x = xDirection;
                identify->faceTo = (xDirection > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);

                return BattleConfig::MOVE_SOLDIER_NEXT;
            }
            else
            {
                return BattleConfig::MOVE_SOLDIER_BLOCK;
            }
        }
    }
}

enum BattleConfig::MoveSoldierResult PathFinding::moveCenter(entityx::Entity& entity)
{
    auto direction = entity.component<BattleComponent::Direction>();
    auto position = entity.component<BattleComponent::Position>();
    auto target = entity.component<BattleComponent::Target>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    if(arriveMiddle(position)) return BattleConfig::MOVE_SOLDIER_ARRIVE;
    
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    
    target->x = position->x;
    target->y = position->y;
    target->z = position->z;
    
    direction->x = 0;
    direction->y = 0;
    direction->z = 0;
    
    int xDirection = (identify->side == BattleConfig::SIDE_LEFT) ? 1 : -1;
    
    if(myTileZ < STAGE_MIDDLE_Z)
    {
        //down 0-9
        if(myTileZ + 1 < STAGE_MIDDLE_Z && m_entities->getTarget(myTileZ + 1, myTileX) == 0)// 6-9 ok
        {
            target->z = MapHelper::getCenterZ(myTileZ + 1);
            direction->z = 1;
            return BattleConfig::MOVE_SOLDIER_NEXT;
        }
        else
        {
            //down 0-9 // 6-9 ok
            if(myTileZ >= STAGE_MIDDLE_DOWN) return BattleConfig::MOVE_SOLDIER_ARRIVE;
            
            if(myTileX + xDirection < STAGE_WIDTH && myTileX + xDirection > -1 && m_entities->getTarget(myTileZ, myTileX + xDirection) == 0)
            {
                target->x = MapHelper::getCenterX(myTileX + xDirection, false);
                direction->x = xDirection;
                identify->faceTo = (xDirection > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
               
                return BattleConfig::MOVE_SOLDIER_NEXT;
            }
            else
            {
                return BattleConfig::MOVE_SOLDIER_BLOCK;
            }
        }
    }
    else
    {
        //up 10-19
        if(myTileZ - 1 >= STAGE_MIDDLE_Z && m_entities->getTarget(myTileZ - 1, myTileX) == 0)// 10-14 ok
        {
            target->z = MapHelper::getCenterZ(myTileZ - 1);
            direction->z = -1;
            return BattleConfig::MOVE_SOLDIER_NEXT;
        }
        else
        {
            //up 10-19 // 10-13
            if(myTileZ < STAGE_MIDDLE_UP) return BattleConfig::MOVE_SOLDIER_ARRIVE;
            
            if(myTileX + xDirection < STAGE_WIDTH && myTileX + xDirection > -1 && m_entities->getTarget(myTileZ, myTileX + xDirection) == 0)
            {
                target->x = MapHelper::getCenterX(myTileX + xDirection, false);
                direction->x = xDirection;
                identify->faceTo = (xDirection > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
                
                return BattleConfig::MOVE_SOLDIER_NEXT;
            }
            else
            {
                return BattleConfig::MOVE_SOLDIER_BLOCK;
            }
        }
    }
}

enum BattleConfig::MoveSoldierResult PathFinding::moveBack(entityx::Entity& entity)
{
    auto direction = entity.component<BattleComponent::Direction>();
    auto position = entity.component<BattleComponent::Position>();
    auto target = entity.component<BattleComponent::Target>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    
    target->x = position->x;
    target->y = position->y;
    target->z = position->z;
    
    direction->x = 0;
    direction->y = 0;
    direction->z = 0;
    
    if((myTileX <= 1 && identify->side == BattleConfig::SIDE_LEFT) ||
       (myTileX >= STAGE_WIDTH - 2 && identify->side == BattleConfig::SIDE_RIGHT))
    {
        return BattleConfig::MOVE_SOLDIER_ARRIVE;
    }
    
    int xDirection = (identify->side == BattleConfig::SIDE_LEFT) ? -1 : 1;
    if(m_entities->getTarget(myTileZ, myTileX + xDirection) == 0)
    {
        target->x = MapHelper::getCenterX(myTileX + xDirection, false);
        direction->x = xDirection;
        identify->faceTo = (xDirection > 0 ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
        
        return BattleConfig::MOVE_SOLDIER_NEXT;
    }
    else
    {
        int total = 0;
        int zDirection = 0;
        if(myTileZ > 0 && m_entities->getTarget(myTileZ - 1, myTileX) == 0)
        {
            zDirection = -1;
            total++;
        }
        if(myTileZ < STAGE_HEIGHT - 1 && m_entities->getTarget(myTileZ + 1, myTileX) == 0)
        {
            zDirection = 1;
            total++;
        }
        
        if(total == 0) return BattleConfig::MOVE_SOLDIER_BLOCK;
        
        if(total == 2) zDirection = (cocos2d::random() % 2) ? 1 : -1;
        
        target->z = MapHelper::getCenterZ(myTileZ + zDirection);
        direction->z = zDirection;
        
        return BattleConfig::MOVE_SOLDIER_NEXT;
    }
}

bool PathFinding::arriveMiddle(BattleComponent::Position::Handle& position)
{
    int myTileZ = MapHelper::getTileZ(position->z);
    
    if(myTileZ < STAGE_MIDDLE_Z)
    {
        //down 0-9
        if(position->z + HALF_GRID_HEIGHT >= STAGE_MIDDLE_Z * GRID_HEIGHT) return true;
    }
    else
    {
        //up 10-19
        if(position->z - HALF_GRID_HEIGHT <= STAGE_MIDDLE_Z * GRID_HEIGHT) return true;
    }
    return false;
}
