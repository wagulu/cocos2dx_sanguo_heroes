//
//  HeroDistanceLessThan.cpp
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#include "HeroAndHeroDistanceLessThan.h"
#include "BattleWorld.h"

bool HeroAndHeroDistanceLessThan::isOk()
{
    if(shouldCheck())
    {
        int distance = (int)m_data->value;
        auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
        if (pEntity != nullptr)
        {
            auto identify = pEntity->component<BattleComponent::Identify>();
            auto position = pEntity->component<BattleComponent::Position>();
            
            int startTileX = MapHelper::getTileX(position->x);
            int startTileZ = MapHelper::getTileZ(position->z);
            int step, endTileX, id;
            
            if(identify->faceTo == BattleConfig::FACE_TO_RIGHT)
            {
                startTileX += 1;
                step = 1;
                endTileX = startTileX + distance;
            }
            else
            {
                startTileX -= 2;
                step = -1;
                endTileX = startTileX - distance;
            }
            
            for(int i = startTileX; i != endTileX; i += step)
            {
                if(i >= 0 && i < STAGE_WIDTH)
                {
                    id = _BATTLE_ENTITY.getTarget(startTileZ, i);
                    if(id != 0 && MapHelper::getSide(id) != identify->side && MapHelper::getEntityType(id) == BattleConfig::ENTITY_HERO)
                    {
                        return true;
                    }
                }
            }

        }
    }
    return false;
}
