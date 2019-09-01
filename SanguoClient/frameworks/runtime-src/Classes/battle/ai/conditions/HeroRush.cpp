//
//  HeroGo.cpp
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#include "HeroRush.h"
#include "BattleWorld.h"


void HeroRush::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::StrategyHeroRush>(*this);
}

HeroRush::~HeroRush()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::StrategyHeroRush>(*this);
}


void HeroRush::receive(const BattleEvent::StrategyHeroRush& e)
{
    if(e.entity.component<BattleComponent::Identify>()->id == m_heroId)
    {
        m_ok = true;
    }
}

