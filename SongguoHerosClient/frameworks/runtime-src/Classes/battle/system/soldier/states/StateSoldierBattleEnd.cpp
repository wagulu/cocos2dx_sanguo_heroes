//
//  StateSoldierBattleEnd.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierBattleEnd.h"
#include "AICommonHelper.h"

void StateSoldierBattleEnd::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE);
}

void StateSoldierBattleEnd::exit(entityx::Entity& entity)
{
}

int StateSoldierBattleEnd::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    return m_result;
}
