//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierRecenter__
#define __Game__StateSoldierRecenter__

#include <stdio.h>
#include "../../stateMachine/IState.h"
#include "BattleComponent.h"

class StateSoldierRecenter : public IState
{
public:
    enum RESULT
    {
        WORKING,
//        ATTACK_ENEMY,
//        WIND,
        ARRIVE,
        BOWMAN_ATTACK,
//        WAIT,
//        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierRecenter();
    virtual ~StateSoldierRecenter();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_CENTER;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
//    void tryArriveWindWait(entityx::Entity& entity);

    static bool isArriveCenter(entityx::Entity& entity);
    
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
