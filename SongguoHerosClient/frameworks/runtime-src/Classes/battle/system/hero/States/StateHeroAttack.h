//
//  StateHeroAttack.h
//  Game
//
//  Created by fuchenhao on 2/28/15.
//
//

#ifndef __Game__StateHeroAttack__
#define __Game__StateHeroAttack__

#include <stdio.h>

#include "../../stateMachine/IState.h"
#include "BattleConfig.h"

#define HERO_ATTACK_SPEED_V      500
#define HERO_ATTACK_SPEED_H      200
#define HERO_ATTACK_GRAVITY      1200

class StateHeroAttack : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ENEMY_DEAD,
        ENEMY_OUT_OF_RANGE
    };
    RESULT m_result = WORKING;
    
    StateHeroAttack(int m_lastStateCode);
    virtual ~StateHeroAttack();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_ATTACK;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    int getLastStateCode() { return m_lastStateCode; }
    void sendRevengeEvent(entityx::Entity& entity);

private:
    enum SUB_STATE
    {
        APPROACH,
        COOLDOWN,
        ATTACK,
    };
    SUB_STATE m_subState = APPROACH;
    
    float getAttackDelay();
    void doAttack(entityx::Entity& entity);
    void playAttackAnimation(entityx::Entity& entity, entityx::Entity& targetEntity);
    
    int m_lastStateCode;
    
};

#endif /* defined(__Game__StateHeroAttack__) */
