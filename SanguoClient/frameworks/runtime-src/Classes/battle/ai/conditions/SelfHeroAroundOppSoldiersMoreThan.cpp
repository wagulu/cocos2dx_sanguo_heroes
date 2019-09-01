//
//  SoldiersMoreThan.cpp
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#include "SelfHeroAroundOppSoldiersMoreThan.h"
#include "BattleWorld.h"

bool SelfHeroAroundOppSoldiersMoreThan::isOk()
{
    if(shouldCheck())
    {
        int count = (int)m_data->value;
        int current = 0;
        
        auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
        if (pEntity != nullptr)
        {
            auto identify = pEntity->component<BattleComponent::Identify>();
            auto position = pEntity->component<BattleComponent::Position>();
            
            int tileX = MapHelper::getTileX(position->x);
            int tileZ = MapHelper::getTileZ(position->z);
            int id;
            
            for(int i = tileX - 3; i <= tileX + 2; i++)
            {
                for(int j = tileZ - 2; j <= tileZ + 2; j++)
                {
                    if(i >= 0 && i < STAGE_WIDTH && j >= 0 && j < STAGE_WIDTH)
                    {
                        id = _BATTLE_ENTITY.getTarget(j, i);
                        if(id != 0 && MapHelper::getSide(id) != identify->side && MapHelper::getEntityType(id) == BattleConfig::ENTITY_SOLDIER)
                        {
                            current++;
                        }
                    }
                }
            }
        }
        return current >= count;
    }
    return false;
}
