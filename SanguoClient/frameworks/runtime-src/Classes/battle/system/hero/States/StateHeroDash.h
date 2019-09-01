//
//  StateHeroDash.h
//  SGGame
//
//  Created by fuchenhao on 15-4-28.
//
//

#ifndef __Game__StateHeroDash__
#define __Game__StateHeroDash__

#include "IState.h"
#include "BattleConfig.h"
#include "SimpleTween.h"

class StateHeroDash : public IState
{
public:
    enum RESULT
    {
        WORKING,
        OVER
    };
    
    RESULT m_result = WORKING;
    
    StateHeroDash(const std::string& animation, float duration, float speed);
    virtual ~StateHeroDash();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_DASH;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    bool m_pushHero = false;
    
    float m_elapse = 0;
    float m_duration = 0;
    float m_speed = 8.0;
    
    std::string m_animation;
    
    SimpleTween_linear* m_moveTween = nullptr;
};

#endif /* defined(__Game__StateHeroDash__) */
