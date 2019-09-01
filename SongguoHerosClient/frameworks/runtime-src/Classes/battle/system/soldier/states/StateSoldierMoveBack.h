//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierMoveBack__
#define __Game__StateSoldierMoveBack__

#include <stdio.h>
#include "IState.h"
#include "BattleConfig.h"

class StateSoldierMoveBack : public IState
{
public:
    enum RESULT
    {
        WORKING,
        BOWMAN_ATTACK,
//        ARRIVE,
        //        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierMoveBack();
    virtual ~StateSoldierMoveBack();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_BACK;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    enum SUB_STATE
    {
//        NORMAL,
        WIND,
        WAIT,
        THINK,
    };
    SUB_STATE m_subState = THINK;
    
    float m_waitTimeLeft = 0;
    void toWaitSubState();
    
};

#endif /* defined(__Game__StateSoldierRun__) */
