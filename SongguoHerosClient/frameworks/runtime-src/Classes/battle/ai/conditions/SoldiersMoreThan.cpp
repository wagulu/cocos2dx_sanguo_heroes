//
//  SoldiersMoreThan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#include "SoldiersMoreThan.h"
#include "BattleWorld.h"

bool SoldiersMoreThan::isOk()
{
    auto side = MapHelper::getSide(m_heroId);
    auto num = (side == BattleConfig::SIDE_LEFT) ? _BATTLE_ENTITY.getLeftSoldiers().size() : _BATTLE_ENTITY.getRightSoldiers().size();
    return num >= m_data->value;
}
