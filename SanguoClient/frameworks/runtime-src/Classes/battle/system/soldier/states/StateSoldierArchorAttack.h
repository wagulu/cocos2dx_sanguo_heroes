//
//  StateSoldierArchorAttack.h
//  sanguoClient
//
//  Created by fu.chenhao on 4/16/15.
//
//

#ifndef __sanguoClient__StateSoldierArchorAttack__
#define __sanguoClient__StateSoldierArchorAttack__

#include <stdio.h>
#include "../../stateMachine/IState.h"


class StateSoldierArchorAttack : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ENEMY_INVALID,
        ATTACK_DONE,
        //        ATTACK_ENEMY,
        //        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierArchorAttack();
    virtual ~StateSoldierArchorAttack();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_ANCHOR_ATTACK;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
//    void sendRevengeEvent(entityx::Entity& entity);
    
private:
    enum SUB_STATE
    {
        COOLDOWN,
        ATTACK,
    };
    SUB_STATE m_subState = COOLDOWN;
    
    void doAttack(entityx::Entity& entity);

    float getAttackDelay();
    float m_fireDelay = 0;
};

#endif /* defined(__sanguoClient__StateSoldierArchorAttack__) */
