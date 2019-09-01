//
//  StateSoldierPlayAnimation.cpp
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#include "StateSoldierPlayAnimation.h"

#include "SoldierSystem.h"
#include "../../AICommonHelper.h"

StateSoldierPlayAnimation::StateSoldierPlayAnimation(std::string animation, float duration) : m_animation(animation), m_duration(duration)
{
}

StateSoldierPlayAnimation::~StateSoldierPlayAnimation()
{
    
}

void StateSoldierPlayAnimation::enter(entityx::Entity& entity)
{
    BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();
    
    if(m_duration == 0)
    {
        animation->battleAnimation->playAnimation(m_animation, false);
    }
    else
    {
        animation->battleAnimation->playAnimation(m_animation, true);
        m_elapse = 0;
    }
}

void StateSoldierPlayAnimation::exit(entityx::Entity& entity)
{
    
}

int StateSoldierPlayAnimation::update(entityx::Entity& entity, double dt)
{
    BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();
    
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    if(m_duration == 0)
    {
        if(animation->battleAnimation->isComplete())
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
