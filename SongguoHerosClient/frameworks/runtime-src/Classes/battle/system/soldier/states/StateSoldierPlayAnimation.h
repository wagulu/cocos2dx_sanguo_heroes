//
//  StateSoldierPlayAnimation.h
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#ifndef __STATE_HERO_StateSoldierPlayAnimation__
#define __STATE_HERO_StateSoldierPlayAnimation__

#include <stdio.h>

#include "../../stateMachine/IState.h"
#include "BattleConfig.h"

class StateSoldierPlayAnimation : public IState
{
public:
    enum RESULT
    {
        WORKING,
        DONE
    };
    RESULT m_result = WORKING;
    
    StateSoldierPlayAnimation(std::string animation, float duration = 0);
    virtual ~StateSoldierPlayAnimation();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_PLAY_ANIMATION;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    float m_elapse;
    float m_duration;
    std::string m_animation;
};

#endif /* defined(__STATE_HERO_StateSoldierPlayAnimation__) */
