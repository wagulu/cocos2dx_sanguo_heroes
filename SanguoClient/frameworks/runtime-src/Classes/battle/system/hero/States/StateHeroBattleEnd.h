//
//  StateHeroBattleEnd.h
//  sanguoClient
//
//  Created by fuchenhao on 5/5/15.
//
//

#ifndef __sanguoClient__StateHeroBattleEnd__
#define __sanguoClient__StateHeroBattleEnd__

#include "../../stateMachine/IState.h"

class StateHeroBattleEnd : public IState
{
public:
    enum RESULT
    {
        WORKING
    };
    RESULT m_result = WORKING;
    
    virtual int getStateCode()
    {
        return BattleConfig::GeneralState::GENERAL_BATTLE_END;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
};

#endif /* defined(__sanguoClient__StateHeroBattleEnd__) */
