//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierIdle__
#define __Game__StateSoldierIdle__

#include <stdio.h>
#include "IState.h"

//#define FIND_TARGET_INTERVAL (0.5);

class StateSoldierIdle : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ATTACK_ENEMY,
        ATTACK_ENEMY_HERO,
        MOVE_BACK,
        RECENTER,
        DEPART,
        PROTECT_HERO,
        BOWMAN_ATTACK,
        
        ATTACK_NEARBY_ENEMY,
    };
    RESULT m_result = WORKING;
    
    StateSoldierIdle();
    virtual ~StateSoldierIdle();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_IDLE;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    
//    float m_findTargetTicket;
};

#endif /* defined(__Game__StateSoldierRun__) */
