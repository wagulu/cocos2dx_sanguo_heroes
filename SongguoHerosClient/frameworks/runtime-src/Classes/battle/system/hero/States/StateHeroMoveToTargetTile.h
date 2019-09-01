//
//  StateHeroMoveToTargetTile.h
//  sanguoGame
//
//  Created by fuchenhao on 7/9/15.
//
//

#ifndef __sanguoGame__StateHeroMoveToTargetTile__
#define __sanguoGame__StateHeroMoveToTargetTile__

#include "IState.h"
#include "BattleConfig.h"

class StateHeroMoveToTargetTile : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ARRIVE
    };
    RESULT m_result = WORKING;
    
    StateHeroMoveToTargetTile(const BattleConfig::Tile& targetTile);
    virtual ~StateHeroMoveToTargetTile();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_MOVE_TO_TARGET_TILE;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    float m_targetX;
    float m_targetY;
    float m_targetZ;
};

#endif /* defined(__sanguoGame__StateHeroMoveToTargetTile__) */
