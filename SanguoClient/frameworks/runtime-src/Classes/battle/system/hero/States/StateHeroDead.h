//
//  StateHeroDead.h
//  Game
//
//  Created by fuchenhao on 2/28/15.
//
//

#ifndef __Game__StateHeroDead__
#define __Game__StateHeroDead__

#include "IState.h"
#include "BattleConfig.h"

class StateHeroDead : public IState
{
public:
    enum RESULT
    {
        WORKING,
        DONE,
    };
    RESULT m_result = WORKING;
    
    StateHeroDead(const BattleConfig::AttackData& attackData);
    virtual ~StateHeroDead();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_DEAD;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);

private:
    enum STATE_DEAD
    {
        STATE_WAITING_FLY_TEXT_COMPLETE,
        STATE_WAITING_DEAD_ANIM
    };
    STATE_DEAD m_subState = STATE_WAITING_FLY_TEXT_COMPLETE;

    BattleConfig::AttackData m_attackBringToDeath;
    bool m_killedBySkill = false;
    
    void playSound(entityx::Entity& entity);
};

#endif /* defined(__Game__StateHeroDead__) */
