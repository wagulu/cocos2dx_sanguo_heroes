//
//  StateHeroRun.h
//  Game
//
//  Created by fuchenhao on 15-2-27.
//
//

#ifndef __Game__StateHeroRun__
#define __Game__StateHeroRun__

#include <stdio.h>

#include "../../stateMachine/IState.h"
#include "BattleConfig.h"

class StateHeroRun : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ATTACK_ENEMY,
        NEED_CHANGE_LINE,
    };
    RESULT m_result = WORKING;
    
    StateHeroRun();
    virtual ~StateHeroRun();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_RUN;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    bool m_isMainHero;
    
    int m_audioId;
    
};

#endif /* defined(__Game__StateHeroRun__) */
