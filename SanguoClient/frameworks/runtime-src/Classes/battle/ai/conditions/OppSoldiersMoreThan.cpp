//
//  OppSoldiersMoreThan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#include "OppSoldiersMoreThan.h"
#include "BattleWorld.h"

bool OppSoldiersMoreThan::isOk()
{
    auto side = MapHelper::getSide(m_heroId);
    auto num = (side == BattleConfig::SIDE_LEFT) ? _BATTLE_ENTITY.getRightSoldiers().size() : _BATTLE_ENTITY.getLeftSoldiers().size();
    return num >= m_data->value;
}
