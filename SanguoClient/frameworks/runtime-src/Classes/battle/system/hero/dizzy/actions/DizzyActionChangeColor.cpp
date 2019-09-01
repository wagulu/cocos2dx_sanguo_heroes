//
//  DizzyActionChangeColor.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#include "DizzyActionChangeColor.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

DizzyActionChangeColor::~DizzyActionChangeColor()
{
    
}

void DizzyActionChangeColor::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    animation->m_tweenQueue.pushTween(TweenQueue::TARGET::R, TweenQueue::OPERATION::IN,
                                      new SimpleTween_linear(0.3, animation->battleAnimation->getColorR(), 0.0), 0);
}

void DizzyActionChangeColor::exit(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    animation->m_tweenQueue.pushTween(TweenQueue::TARGET::R, TweenQueue::OPERATION::IN,
                                      new SimpleTween_linear(0.3, animation->battleAnimation->getColorR(), 1.0), 0);
}

void DizzyActionChangeColor::update(entityx::Entity& entity, float dt)
{
    
}
