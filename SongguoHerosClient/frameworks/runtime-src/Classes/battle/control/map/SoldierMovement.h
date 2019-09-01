//
//  SoldierMovement.h
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#ifndef __SOLDIER_MOVEMENT_H__
#define __SOLDIER_MOVEMENT_H__

#include "BattleEntity.h"
#include "../../BattleStage.h"
#include "MapHelper.h"

class SoldierMovement
{
public:
    
    SoldierMovement(BattleEntity* entities, BattleStage* stage);
    ~SoldierMovement();
    
    void updateDirection(entityx::Entity& entity);
    bool move(float dt, entityx::Entity& entity);
    bool move2(float dt, entityx::Entity& entity);
    bool moveTo(float x, float y, float z, entityx::Entity& entity);
    bool arrivePosition(float dt, entityx::Entity& entity);
    
    bool canAttack(entityx::Entity& entity, entityx::Entity& targetEntity);
    bool moveApproachAttackRange(float dt, entityx::Entity& entity, entityx::Entity& targetEntity);//return true when approach complete
    
    bool findNearbySoldier(entityx::Entity& entity);
    
    void knockBack(entityx::Entity& entity, float directX, float expectDist);
    
    bool isSoldierPositionLostInTileMap(int soldierId, float z, float x);

    bool canBowmanAttack(entityx::Entity& entity, entityx::Entity& targetEntity);

private:
    
    BattleEntity* m_entities;
    BattleStage* m_stage;
};

#endif /* defined(__SOLDIER_MOVEMENT_H__) */
