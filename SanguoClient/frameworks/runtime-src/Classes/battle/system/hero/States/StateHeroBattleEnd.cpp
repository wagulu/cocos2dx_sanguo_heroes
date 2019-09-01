//
//  StateHeroBattleEnd.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/5/15.
//
//

#include "StateHeroBattleEnd.h"
#include "AICommonHelper.h"

void StateHeroBattleEnd::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE);
}

void StateHeroBattleEnd::exit(entityx::Entity& entity)
{
}

int StateHeroBattleEnd::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    return m_result;
}
