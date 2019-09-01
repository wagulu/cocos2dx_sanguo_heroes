//
//  StateSoldierBattleEnd.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierRun__
#define __Game__StateSoldierRun__

#include "../../stateMachine/IState.h"

class StateSoldierBattleEnd : public IState
{
public:
    enum RESULT
    {
        WORKING
    };
    RESULT m_result = WORKING;
    
    virtual int getStateCode()
    {
        return BattleConfig::SoldierState::STATE_BATTLE_END;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
};

#endif /* defined(__Game__StateSoldierRun__) */
