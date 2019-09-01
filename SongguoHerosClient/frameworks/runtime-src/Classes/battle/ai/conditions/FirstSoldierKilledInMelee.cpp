//
//  FirstSoldierKilledInMelee.cpp
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#include "FirstSoldierKilledInMelee.h"
#include "BattleWorld.h"

void FirstSoldierKilledInMelee::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::BattleActorDead>(*this);
}

FirstSoldierKilledInMelee::~FirstSoldierKilledInMelee()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::BattleActorDead>(*this);
}

void FirstSoldierKilledInMelee::receive(const BattleEvent::BattleActorDead& e)
{
    auto targetIdentify = e.entity.component<BattleComponent::Identify>();
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity != nullptr)
    {
        auto heroIdentify = pEntity->component<BattleComponent::Identify>();
        
        if(targetIdentify->side != heroIdentify->side && targetIdentify->type == BattleConfig::ENTITY_SOLDIER)
        {
            if(e.attackData.getAttackType() == BattleConfig::AttackType::NORMAL)
            {
                m_ok = true;
            }
        }
    }
}
