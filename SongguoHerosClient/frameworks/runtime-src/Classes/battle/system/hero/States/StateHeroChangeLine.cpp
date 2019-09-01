//
//  StateHeroChangeLine.cpp
//  Game
//
//  Created by fuchenhao on 3/26/15.
//
//

#include "StateHeroChangeLine.h"
#include "../../AICommonHelper.h"

StateHeroChangeLine::StateHeroChangeLine()
{
    
}

StateHeroChangeLine::~StateHeroChangeLine()
{
    
}

void StateHeroChangeLine::enter(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    int targetId = (identify->side == BattleConfig::SIDE_LEFT ? RIGHT_HERO : LEFT_HERO);
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity != nullptr)
    {
        if (entity.has_component<BattleComponent::TargetEntity>())
        {
            entity.component<BattleComponent::TargetEntity>()->targetEntity = *pTargetEntity;
        }
        else
        {
            entity.assign<BattleComponent::TargetEntity>(*pTargetEntity);
        }
        
        m_result = MOVE_TO_HERO_BACK;
    }
    else
    {
        m_result = FAIL;
    }
}

void StateHeroChangeLine::exit(entityx::Entity& entity)
{
    
}

int StateHeroChangeLine::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    return m_result;
}
