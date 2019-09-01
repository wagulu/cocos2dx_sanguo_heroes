//
//  HeroAndSoldierDistanceLessThan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#include "HeroAndSoldierDistanceLessThan.h"
#include "BattleWorld.h"

bool HeroAndSoldierDistanceLessThan::isOk()
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
        
        //recaculate by hero position
        auto* pMyEntity = _BATTLE_ENTITY.getEntity(m_heroId);
        if (pMyEntity != nullptr)
        {
            auto myHeroSide = pMyEntity->component<BattleComponent::Identify>()->side;
            auto targetHeroX = MapHelper::getTileX(pMyEntity->component<BattleComponent::Position>()->x);
            if(myHeroSide == BattleConfig::SIDE_LEFT)
            {
                maxLeft = targetHeroX;
            }
            else
            {
                minRight = targetHeroX;
            }
            
            if(minRight <= maxLeft || (minRight - maxLeft) <= distance)
            {
                return true;
            }
        }
    }
    return false;
}
