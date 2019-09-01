//
//  HeroHp.cpp
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#include "HeroHpLessThan.h"
#include "BattleWorld.h"

bool HeroHpLessThan::isOk()
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity != nullptr)
    {
        auto property = pEntity->component<BattleComponent::Property>();
        return property->get(BattleConfig::Property::HP) <= m_data->value;
    }
    return false;
}
