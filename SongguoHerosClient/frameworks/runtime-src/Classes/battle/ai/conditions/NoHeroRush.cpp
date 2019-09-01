//
//  NoHeroRush.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#include "NoHeroRush.h"
#include "BattleWorld.h"

void NoHeroRush::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::StrategyHeroRush>(*this);
    m_ok = true;
}

NoHeroRush::~NoHeroRush()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::StrategyHeroRush>(*this);
}

void NoHeroRush::receive(const BattleEvent::StrategyHeroRush& e)
{
    if (e.entity.component<BattleComponent::Identify>()->id == m_heroId)
    {
        m_ok = false;
    }
}
