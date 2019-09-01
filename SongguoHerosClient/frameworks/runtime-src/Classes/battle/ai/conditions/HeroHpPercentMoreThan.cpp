//
//  HeroHpPercentMoreThan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#include "HeroHpPercentMoreThan.h"
#include "BattleWorld.h"

bool HeroHpPercentMoreThan::isOk()
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity != nullptr)
    {
        auto property = pEntity->component<BattleComponent::Property>();
        return property->get(BattleConfig::Property::HP) / property->getOriginal(BattleConfig::Property::HP) >= m_data->value;
    }
    return false;
}
