//
//  StateHeroIdle.h
//  Game
//
//  Created by fu.chenhao on 2/26/15.
//
//

#ifndef __Game__StateHeroIdle__
#define __Game__StateHeroIdle__

#include <stdio.h>

#include "../../stateMachine/IState.h"
#include "BattleConfig.h"

class StateHeroIdle : public IState
{
public:
    enum RESULT
    {
        WORKING,
        RUSH_OUT,
        ATTACK_ENEMY,
    };
    RESULT m_result = WORKING;
    
    StateHeroIdle();
    virtual ~StateHeroIdle();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_IDLE;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
};

#endif /* defined(__Game__StateHeroIdle__) */
