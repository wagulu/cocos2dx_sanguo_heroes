//
//  StateHeroPlayAnimation.h
//  Game
//
//  Created by fu.chenhao on 3/12/15.
//
//

#ifndef __STATE_HERO_StateHeroPlayAnimation__
#define __STATE_HERO_StateHeroPlayAnimation__

#include "IState.h"
#include "BattleConfig.h"

class StateHeroPlayAnimation : public IState
{
public:
    enum RESULT
    {
        WORKING,
        DONE
    };
    RESULT m_result = WORKING;
    
    StateHeroPlayAnimation(std::string animation, float duration = 0);
    virtual ~StateHeroPlayAnimation();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_PLAY_ANIMATION;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    float m_elapse;
    float m_duration;
    std::string m_animation;
};

#endif /* defined(__STATE_HERO_StateHeroPlayAnimation__) */
