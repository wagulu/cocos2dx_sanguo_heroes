//
//  StateHeroMoveToHeroBack.h
//  Game
//
//  Created by fuchenhao on 3/26/15.
//
//

#ifndef __Game__StateHeroMoveToHeroBack__
#define __Game__StateHeroMoveToHeroBack__

#include "../../stateMachine/IState.h"
#include "BattleConfig.h"

class StateHeroMoveToHeroBack : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ARRIVE,
        TARGET_HERO_DEAD,
        ATTACK_ENEMY
    };
    RESULT m_result = WORKING;
    
    StateHeroMoveToHeroBack();
    virtual ~StateHeroMoveToHeroBack();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_MOVE_TO_HERO_BACK;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
};

#endif /* defined(__Game__StateHeroMoveToHeroBack__) */
