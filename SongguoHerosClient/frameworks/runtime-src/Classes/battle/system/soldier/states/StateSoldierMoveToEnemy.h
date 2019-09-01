//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierMoveToEnemy__
#define __Game__StateSoldierMoveToEnemy__

#include <stdio.h>
#include "IState.h"
#include "BattleConfig.h"

class StateSoldierMoveToEnemy : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ATTACK_ENEMY,
        BOWMAN_ATTACK,
        AIMING_ENEMY_HERO,
//        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierMoveToEnemy();
    virtual ~StateSoldierMoveToEnemy();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_SOLDIER;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);

private:
    enum SUB_STATE
    {
        NORMAL,
        TO_WIND,
        WIND,
        WAIT,
    };
    SUB_STATE m_subState = NORMAL;

//    float m_lastResearchTargetTime = 0;
//    bool researchTarget(entityx::Entity& entity);

    float m_waitTimeLeft = 0;
    void toWaitSubState();
    
};

#endif /* defined(__Game__StateSoldierRun__) */
