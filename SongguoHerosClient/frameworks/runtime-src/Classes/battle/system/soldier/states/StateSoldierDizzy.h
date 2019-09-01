//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierDizzy__
#define __Game__StateSoldierDizzy__

#include <stdio.h>
#include "../../stateMachine/IState.h"

class StateSoldierDizzy : public IState
{
public:
    enum RESULT
    {
        WORKING,
        OVER
    };
    RESULT m_result = WORKING;
    
    StateSoldierDizzy(float dizzyTime);
    virtual ~StateSoldierDizzy();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_DIZZY;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    float m_dizzyTime = 0;
    float m_elapse = 0;
    
    entityx::Entity m_effectEntity;
};

#endif /* defined(__Game__StateSoldierRun__) */
