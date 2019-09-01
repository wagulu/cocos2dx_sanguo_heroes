//
//  DizzyActionStopAnimation.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#include "DizzyActionStopAnimation.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

DizzyActionStopAnimation::~DizzyActionStopAnimation()
{
    
}

void DizzyActionStopAnimation::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, false);
    animation->battleAnimation->pauseCurrent(true);
}

void DizzyActionStopAnimation::exit(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
    animation->battleAnimation->pauseCurrent(false);
}

void DizzyActionStopAnimation::update(entityx::Entity& entity, float dt)
{
    
}
