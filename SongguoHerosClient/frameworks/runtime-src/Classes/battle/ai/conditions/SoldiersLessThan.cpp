//
//  SoldierLessThan.cpp
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#include "SoldiersLessThan.h"
#include "BattleWorld.h"

bool SoldiersLessThan::isOk()
{
    auto side = MapHelper::getSide(m_heroId);
    auto num = (side == BattleConfig::SIDE_LEFT) ? _BATTLE_ENTITY.getLeftSoldiers().size() : _BATTLE_ENTITY.getRightSoldiers().size();
    return num <= m_data->value;
}
