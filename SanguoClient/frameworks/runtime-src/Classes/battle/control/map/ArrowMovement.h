//
//  ArrowMovement.h
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#ifndef __ARROW_MOVEMENT_H__
#define __ARROW_MOVEMENT_H__

#include "BattleEntity.h"
#include "MapHelper.h"

class ArrowMovement
{
public:
    ArrowMovement();
    ~ArrowMovement();
    
    BattleConfig::MoveArrowResult moveLine(float dt, entityx::Entity& entity);
    BattleConfig::MoveArrowResult moveVertical(float dt, entityx::Entity& entity);
    BattleConfig::MoveArrowResult moveCurve(float dt, entityx::Entity& entity);
    BattleConfig::MoveArrowResult moveCircle(float dt, entityx::Entity& entity);
    
    void updateDirection(float dt, entityx::Entity& entity);
};

#endif /* defined(__ARROW_MOVEMENT_H__) */

