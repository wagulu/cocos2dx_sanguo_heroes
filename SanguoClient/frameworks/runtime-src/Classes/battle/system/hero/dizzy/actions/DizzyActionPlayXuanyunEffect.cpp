//
//  DizzyActionPlayXuanyunEffect.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#include "DizzyActionPlayXuanyunEffect.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

DizzyActionPlayXuanyunEffect::~DizzyActionPlayXuanyunEffect()
{
    
}

void DizzyActionPlayXuanyunEffect::enter(entityx::Entity& entity)
{
    auto* pEffectGroup = _BATTLE_ANIMATION_CREATOR.convertAnimationToEffectGroup("battleEffect", "animation_xuanyun2");
    _ENTITY_EVENT.emit<BattleEvent::AddEntityAnimation>(pEffectGroup, entity, false);
    
    CC_SAFE_DELETE(pEffectGroup);
}

void DizzyActionPlayXuanyunEffect::exit(entityx::Entity& entity)
{
    _ENTITY_EVENT.emit<BattleEvent::RemoveEntityAnimation>(entity);
}

void DizzyActionPlayXuanyunEffect::update(entityx::Entity& entity, float dt)
{
    
}
