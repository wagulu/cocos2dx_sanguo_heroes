//
//  StateMachine.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateMachine.h"

StateMachine::StateMachine(entityx::Entity entity)
{
    m_entity = entity;
    m_pCurrentState = NULL;
}

StateMachine::~StateMachine()
{
    delete m_pCurrentState;
    m_pCurrentState = NULL;
}

int StateMachine::update(double dt)
{
    if (m_pCurrentState != NULL)
    {
        m_pCurrentState->delegeteUpdate(m_entity, dt);
        
        if (m_pCurrentState->m_pDelegateState == NULL)
        {
            return m_pCurrentState->update(m_entity, dt);
        }
    }
    return -1;
}

void StateMachine::changeState(IState* pNewState, bool acceptDelegate)
{
    if (m_pCurrentState != NULL)
    {
        if (!m_pCurrentState->isDelegateState() || !acceptDelegate)
        {
            m_pCurrentState->delegeteExit(m_entity);
            m_pCurrentState->exit(m_entity);
            delete m_pCurrentState;
            m_pCurrentState = NULL;
        }
    }
    
    IState* pDelegateState = m_pCurrentState;
    
    m_pCurrentState = pNewState;
    if (m_pCurrentState != NULL)
    {
        m_pCurrentState->enter(m_entity);
        
        if (pDelegateState != NULL)
        {
            m_pCurrentState->setDelegateState(pDelegateState);
        }
    }
}

