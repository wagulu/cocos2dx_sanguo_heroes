//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierDepart__
#define __Game__StateSoldierDepart__

#include <stdio.h>
#include "../../stateMachine/IState.h"

class StateSoldierDepart : public IState
{
public:
    enum RESULT
    {
        WORKING,
        BOWMAN_ATTACK,
//        ATTACK_ENEMY,
        //        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierDepart();
    virtual ~StateSoldierDepart();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_DEPART;
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
