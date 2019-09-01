//
//  StateSoldierLost.h
//  sanguoClient
//
//  Created by fuchenhao on 4/1/15.
//
//

#ifndef __sanguoClient__StateSoldierLost__
#define __sanguoClient__StateSoldierLost__

#include "../../stateMachine/IState.h"

class StateSoldierLost : public IState
{
public:
    enum RESULT
    {
        WORKING,
        MOVED_TO_NEW_TILE,
        RESTORE,
        NOT_LOST,
    };
    RESULT m_result = WORKING;
    
    StateSoldierLost();
    virtual ~StateSoldierLost();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_LOST;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
private:
    bool m_block;
    float m_randomRunDelay;
    float m_tileTargetLastX;
    float m_tileTargetLastZ;
};

#endif /* defined(__sanguoClient__StateSoldierLost__) */
