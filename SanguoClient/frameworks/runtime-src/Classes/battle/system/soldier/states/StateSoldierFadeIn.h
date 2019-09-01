//
//  StateSoldierFadeIn.h
//  sanguoClient
//
//  Created by fu.chenhao on 4/17/15.
//
//

#ifndef __sanguoClient__StateSoldierFadeIn__
#define __sanguoClient__StateSoldierFadeIn__

#include <stdio.h>
#include "../../stateMachine/IState.h"
#include "SimpleTween.h"

class StateSoldierFadeIn : public IState
{
public:
    enum RESULT
    {
        WORKING,
        OVER,
        //        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierFadeIn(float dizzyTime);
    virtual ~StateSoldierFadeIn();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_FADEIN;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    SimpleTween_linear* m_moveTween = nullptr;
    float m_totalTime = 0.0f;
    float m_elapse = 0.0f;
    float m_inTime = 0.0f;
};

#endif /* defined(__sanguoClient__StateSoldierFadeIn__) */
