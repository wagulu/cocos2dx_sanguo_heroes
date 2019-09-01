//
//  StateHeroIdle.cpp
//  Game
//
//  Created by fu.chenhao on 2/26/15.
//
//

#include "StateHeroIdle.h"
#include "HeroSystem.h"
#include "../../AICommonHelper.h"

StateHeroIdle::StateHeroIdle()
{
}

StateHeroIdle::~StateHeroIdle()
{
}

void StateHeroIdle::enter(entityx::Entity& entity)
{
}

void StateHeroIdle::exit(entityx::Entity& entity)
{
}

int StateHeroIdle::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto animation = entity.component<BattleComponent::Animation>();
    auto strategy = entity.component<BattleComponent::HeroStrategy>();
    
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE);
    
    switch(strategy->type)
    {
        case StrategyControl::HERO_RUSH_OUT:
            _RETURN_STATE_RESULT(RUSH_OUT);
            break;
            
        default:
            if(_TARGET_FINDER.findHeroNearbyTarget(entity) > 0)
            {
                _RETURN_STATE_RESULT(ATTACK_ENEMY);
            }
            break;
    }
    
    return m_result;
}
