//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierMoveToEnemyHero__
#define __Game__StateSoldierMoveToEnemyHero__

#include <stdio.h>
#include "IState.h"
#include "BattleConfig.h"

class StateSoldierMoveToEnemyHero : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ATTACK_ENEMY,
        BOWMAN_ATTACK,
        FIND_NEARBY_ENEMY_SOLDIER,
        //        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierMoveToEnemyHero();
    virtual ~StateSoldierMoveToEnemyHero();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_GENERAL;
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
        RANDOM,
        WAIT,
    };
    SUB_STATE m_subState = NORMAL;
    
//    float getAttackDelay();
    
    //    float m_elapsed = 0;
    
    float m_waitTimeLeft = 0;
    void toWaitSubState();

};


#endif /* defined(__Game__StateSoldierRun__) */
