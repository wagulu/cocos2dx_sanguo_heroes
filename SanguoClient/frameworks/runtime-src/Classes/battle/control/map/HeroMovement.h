//
//  HeroMovement.h
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#ifndef __HERO_MOVEMENT_H__
#define __HERO_MOVEMENT_H__

#include "BattleEntity.h"
#include "MapHelper.h"
#include "../../BattleEvent.h"
#include "../../BattleStage.h"

class HeroMovement
{
public:
    enum MoveIgnoreType
    {
        IGNORE_SELF_SOLDIER,
        IGNORE_ENEMY_SOLDIER,
        IGNORE_ALL_SOLDIER
    };
    
    HeroMovement(BattleEntity* entities, BattleStage* stage);
    ~HeroMovement();
    
    bool move(float dt, entityx::Entity& entity, MoveIgnoreType ignoreType = IGNORE_SELF_SOLDIER, float speed = 0);
    bool move(float dt, float targetX, float targetY, float targetZ, entityx::Entity& entity, MoveIgnoreType ignoreType = IGNORE_SELF_SOLDIER, float speed = 0);
    bool moveTo(float x, float y, float z, entityx::Entity& entity, MoveIgnoreType ignoreType = IGNORE_SELF_SOLDIER);
    
    bool canAttack(entityx::Entity& entity, entityx::Entity& targetEntity);
    bool moveApproachAttackRange(float dt, entityx::Entity& entity, entityx::Entity& targetEntity);//return true when approach complete
    
private:
    BattleEntity* m_entities;
    BattleStage* m_stage;
};

#endif /* defined(__HERO_MOVEMENT_H__) */
