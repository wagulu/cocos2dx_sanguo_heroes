//
//  DizzyConditionByActionComplete.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/7/15.
//
//

#include "DizzyConditionByActionComplete.h"
#include "BattleWorld.h"
#include "StateHeroDizzy.h"

DizzyConditionByActionComplete::~DizzyConditionByActionComplete()
{
}

void DizzyConditionByActionComplete::init(entityx::Entity& entity)
{
    auto objectStateMachine = entity.component<BattleComponent::ObjectStateMachine>();
    auto* pDizzyState = (StateHeroDizzy*)objectStateMachine->getCurrentState();
    if (pDizzyState)
    {
        m_dizzyActions = pDizzyState->getActions();
    }
}

void DizzyConditionByActionComplete::update(entityx::Entity& entity, float dt)
{
    if (!m_complete)
    {
        m_complete = true;
        
        for (auto* pAction : m_dizzyActions)
        {
            if (!pAction->isComplete())
            {
                m_complete = false;
                break;
            }
        }
    }
}
