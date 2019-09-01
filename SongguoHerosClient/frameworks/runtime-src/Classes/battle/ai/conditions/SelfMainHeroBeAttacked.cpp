//
//  SelfMainHeroBeAttacked.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/9/15.
//
//

#include "SelfMainHeroBeAttacked.h"
#include "BattleWorld.h"

void SelfMainHeroBeAttacked::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::BeAttacked>(*this);
}

SelfMainHeroBeAttacked::~SelfMainHeroBeAttacked()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::BeAttacked>(*this);
}

void SelfMainHeroBeAttacked::receive(const BattleEvent::BeAttacked& e)
{
    auto targetIdentify = e.entity.component<BattleComponent::Identify>();
    auto* pMyEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pMyEntity != nullptr)
    {
        auto myIdentify = pMyEntity->component<BattleComponent::Identify>();
        if (targetIdentify->side == myIdentify->side && MapHelper::isMainHero(targetIdentify->id))
        {
            m_ok = true;
        }
    }
}
