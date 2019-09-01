//
//  DizzyActionPlayHitAnimation.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#include "DizzyActionPlayHitAnimation.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

DizzyActionPlayHitAnimation::~DizzyActionPlayHitAnimation()
{
    
}

void DizzyActionPlayHitAnimation::enter(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_HIT, true);
}

void DizzyActionPlayHitAnimation::exit(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
}

void DizzyActionPlayHitAnimation::update(entityx::Entity& entity, float dt)
{
    
}
