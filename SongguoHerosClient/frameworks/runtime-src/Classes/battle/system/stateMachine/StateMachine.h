//
//  StateMachine.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateMachine__
#define __Game__StateMachine__

#include <stdio.h>
#include "IState.h"
#include "../../../entityx/entityx.h"

class StateMachine
{
public:
    StateMachine(entityx::Entity entity);
    virtual ~StateMachine();
    
    int update(double dt);
    void changeState(IState* pNewState, bool acceptDelegate);
    
    inline IState* getCurrentState() { return m_pCurrentState; };

    inline int getCurrentStateCode()
    {
        return (m_pCurrentState != NULL)? m_pCurrentState->getStateCode(): 0;
    }
    
private:
    entityx::Entity m_entity;
    
    IState* m_pCurrentState;
};

#endif /* defined(__Game__StateMachine__) */
