//
//  StateSoldierHitBack.h
//  sanguoGame
//
//  Created by fuchenhao on 7/29/15.
//
//

#ifndef __sanguoGame__StateSoldierHitBack__
#define __sanguoGame__StateSoldierHitBack__

#include <stdio.h>
#include "IState.h"
#include "BattleConfig.h"

class StateSoldierHitBack : public IState
{
public:
    enum RESULT
    {
        WORKING,
        DONE
    };
    
    RESULT m_result = WORKING;
    
    StateSoldierHitBack(float duration);
    virtual ~StateSoldierHitBack();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_HIT_BACK;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    float m_totalTime = 2.0;
    float m_elapsedTime = 0;
};


#endif /* defined(__sanguoGame__StateSoldierHitBack__) */
