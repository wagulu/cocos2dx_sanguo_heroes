//
//  StateHeroChangeLine.h
//  Game
//
//  Created by fuchenhao on 3/26/15.
//
//

#ifndef __Game__StateHeroChangeLine__
#define __Game__StateHeroChangeLine__

#include "../../stateMachine/IState.h"
#include "BattleConfig.h"

class StateHeroChangeLine : public IState
{
public:
    enum RESULT
    {
        WORKING,
        MOVE_TO_HERO_BACK,
        FAIL,
    };
    RESULT m_result = WORKING;
    
    StateHeroChangeLine();
    virtual ~StateHeroChangeLine();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_CHANGE_LINE;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
};

#endif /* defined(__Game__StateHeroChangeLine__) */
