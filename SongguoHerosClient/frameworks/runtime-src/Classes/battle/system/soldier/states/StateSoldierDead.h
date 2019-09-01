//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierDead__
#define __Game__StateSoldierDead__

#include <stdio.h>
#include "../../stateMachine/IState.h"
#include "BattleComponent.h"

#define _G_             1200.0
#define _V_SPEED_       500.0
#define _H_SPEED_       150.0

class StateSoldierDead : public IState
{
public:
    enum RESULT
    {
        WORKING,
        DONE,
    };
    RESULT m_result = WORKING;
    
    StateSoldierDead(const BattleConfig::AttackData& attackData, bool atOnce = false, bool needRebirth = false, uint64_t deadCauseID = -1, float delay = 0.0);
    StateSoldierDead();
    virtual ~StateSoldierDead();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_DEAD;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    enum STATE_DEAD
    {
        STATE_WAITING_FLY_TEXT_COMPLETE,
        STATE_WAITING_FALLDOWN,
        STATE_WAITING_DEAD_ANIM,
    };
    STATE_DEAD m_subState = STATE_WAITING_FLY_TEXT_COMPLETE;
//    STATE_DEAD m_subState = STATE_WAITING_FALLDOWN;

    void playSound(entityx::Entity& entity);
    
    BattleConfig::DamagePosition m_damagePosition;
    
    float m_delayTime = 0.0;
    
    bool m_atOnce = false;
    bool m_needRebirth = false;
    uint64_t m_deadCauseID;
};

#endif /* defined(__Game__StateSoldierRun__) */
