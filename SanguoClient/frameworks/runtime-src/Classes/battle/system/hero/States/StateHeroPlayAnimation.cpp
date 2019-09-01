//
//  StateHeroPlayAnimation.cpp
//  Game
//
//  Created by fu.chenhao on 3/12/15.
//
//

#include "StateHeroPlayAnimation.h"
#include "HeroSystem.h"
#include "AICommonHelper.h"

StateHeroPlayAnimation::StateHeroPlayAnimation(std::string animation, float duration) : m_animation(animation), m_duration(duration)
{
}

StateHeroPlayAnimation::~StateHeroPlayAnimation()
{
    
}

void StateHeroPlayAnimation::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();

    if (m_duration == 0)
    {
        animation->battleAnimation->playAnimation(m_animation, false);
    }
    else
    {
        animation->battleAnimation->playAnimation(m_animation, true);
        m_elapse = 0;
    }
}

void StateHeroPlayAnimation::exit(entityx::Entity& entity)
{
    
}

int StateHeroPlayAnimation::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    if (m_duration == 0)
    {
        auto animation = entity.component<BattleComponent::Animation>();
        if (animation->battleAnimation->isComplete())
        {
            m_result = DONE;
        }
    }
    else
    {
        m_elapse += dt;
        if(m_duration <= m_elapse)
        {
            m_result = DONE;
        }
    }
    
    return m_result;
}
