//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierProtectHero__
#define __Game__StateSoldierProtectHero__

#include <stdio.h>
#include "IState.h"
#include "BattleConfig.h"

class StateSoldierProtectHero : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ARRIVE,
        HERO_NOT_EXIST,
        BOWMAN_ATTACK,
        //        WAIT,
        //        WIND,
        //        RANDOM_RUN,
    };
    RESULT m_result = WORKING;
    
    StateSoldierProtectHero();
    virtual ~StateSoldierProtectHero();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_PROTECT;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    static bool isArriveHero(entityx::Entity& entity);
    
private:
    enum SUB_STATE
    {
        NORMAL,
        TO_WIND,
        WIND,
        WAIT,
        RANDOM,
    };
    SUB_STATE m_subState = NORMAL;
    
    //    void tryToWindOrRandomRun(entityx::Entity& entity);
    
    float m_waitTimeLeft = 0;
    void toWaitSubState();
    
    
};

#endif /* defined(__Game__StateSoldierRun__) */
