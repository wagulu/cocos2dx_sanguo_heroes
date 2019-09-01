//
//  HeroHpPercentLessThan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/8/15.
//
//

#include "HeroHpPercentLessThan.h"
#include "BattleWorld.h"

bool HeroHpPercentLessThan::isOk()
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity != nullptr)
    {
        auto property = pEntity->component<BattleComponent::Property>();
        return property->get(BattleConfig::Property::HP) / property->getOriginal(BattleConfig::Property::HP) <= m_data->value;
    }
    return false;
}
