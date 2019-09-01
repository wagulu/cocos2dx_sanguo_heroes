//
//  IState.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__IState__
#define __Game__IState__

#include "entityx.h"
#include "BattleConfig.h"

class IState
{
public:
    friend class StateMachine;
    
    IState(){};
    virtual ~IState()
    {
        delegeteFree();
    };
    
    virtual int getStateCode() = 0;
    virtual void enter(entityx::Entity& entity) = 0;
    virtual void exit(entityx::Entity& entity) = 0;
    virtual int update(entityx::Entity& entity, double dt) = 0;
    
    virtual bool isDelegateState() { return false; };
    virtual bool isDelegateStateWorking() { return false; };
    
private:
    void delegeteExit(entityx::Entity& entity)
    {
        if (m_pDelegateState != NULL)
        {
            m_pDelegateState->exit(entity);
            delegeteFree();
        }
    };
    void delegeteFree()
    {
        if (m_pDelegateState != NULL)
        {
            delete m_pDelegateState;
            m_pDelegateState = NULL;
        }
    };
    void delegeteUpdate(entityx::Entity& entity, double dt)
    {
        if (m_pDelegateState != NULL)
        {
            if (m_pDelegateState->isDelegateStateWorking())
            {
                m_pDelegateState->update(entity, dt);
            }
            else
            {
                delegeteExit(entity);
            }
        }
    };
    void setDelegateState(IState* pState)
    {
        m_pDelegateState = pState;
    };

    IState* m_pDelegateState = NULL;
};

#define _RETURN_STATE_RESULT(e) { m_result = (e); return m_result; }

#endif /* defined(__Game__IState__) */
