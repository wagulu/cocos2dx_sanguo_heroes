//
//  EnemyIsClose.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/3/15.
//
//

#include "EnemyIsClose.h"
#include "BattleWorld.h"

bool EnemyIsClose::isOk()
{
    if(shouldCheck())
    {
        int distance = (int)m_data->value;
        
        int maxLeft = 0;
        int minRight = STAGE_WIDTH - 1;
        int current = 0;
        BattleConfig::Side side;
        
        for(int i = 0; i < STAGE_HEIGHT; i++)
        {
            for(int j = 0; j < STAGE_WIDTH; j++)
            {
                current = _BATTLE_ENTITY.getTarget(i, j);
                if(current)
                {
                    side = MapHelper::getSide(current);
                    if(side == BattleConfig::SIDE_LEFT)
                    {
                        if(maxLeft < j) maxLeft = j;
                    }
                    else
                    {
                        if(minRight > j) minRight = j;
                    }
                }
            }
        }
        
        if(minRight <= maxLeft || (minRight - maxLeft) <= distance)
        {
            return true;
        }
    }
    return false;
}
