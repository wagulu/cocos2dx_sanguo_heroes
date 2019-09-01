//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierAttackEnemy__
#define __Game__StateSoldierAttackEnemy__

#include <stdio.h>
#include "../../stateMachine/IState.h"

#define SOLDIER_ATTACK_SPEED_V      300
#define SOLDIER_ATTACK_SPEED_H      100
#define SOLDIER_ATTACK_GRAVITY      1200

class StateSoldierAttackEnemy : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ENEMY_INVALID,
//        ATTACK_ENEMY,
//        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierAttackEnemy();
    virtual ~StateSoldierAttackEnemy();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_ATTACK;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);

    void sendRevengeEvent(entityx::Entity& entity);
    
private:
    enum SUB_STATE
    {
        APPROACH,
        COOLDOWN,
        ATTACK,
    };
    SUB_STATE m_subState = APPROACH;
    
    void doAttack(entityx::Entity& entity);
    float getAttackDelay();
};

#endif /* defined(__Game__StateSoldierRun__) */
