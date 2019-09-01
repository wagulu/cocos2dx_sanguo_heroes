//
//  ActorModeSystem.cpp
//  Game
//
//  Created by fu.chenhao on 3/5/15.
//
//

#include "ActorModeSystem.h"
#include "BattleComponent.h"
#include "ActorMode.h"


void ActorModeSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::SetActorMode>(*this);

}

void ActorModeSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
    BattleComponent::ActorMode::Handle mode;
    
    for (entityx::Entity entity : es.entities_with_components(mode))
    {
        mode->m_modeGroup.update(dt);
    }
    
}


void ActorModeSystem::receive(const BattleEvent::SetActorMode& e)
{
    BattleComponent::ActorMode::Handle mode = e.tarEntity.component<BattleComponent::ActorMode>();
    
    if(!mode.valid())
    {
        return; //return if not a soldier
    }
    
    mode->m_modeGroup.addMode(e.m_type, e.m_duration);

    
}


