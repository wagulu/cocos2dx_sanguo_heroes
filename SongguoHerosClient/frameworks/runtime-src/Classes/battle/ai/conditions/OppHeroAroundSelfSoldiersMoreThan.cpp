//
//  OppHeroAroundSelfSoldiersMoreThan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#include "OppHeroAroundSelfSoldiersMoreThan.h"
#include "BattleWorld.h"

bool OppHeroAroundSelfSoldiersMoreThan::isOk()
{
    if(shouldCheck())
    {
        int count = (int)m_data->value;
        int current = 0;
        
        
        auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
        if (pEntity != nullptr)
        {
            auto identify = pEntity->component<BattleComponent::Identify>();
            int targetId = identify->side == BattleConfig::SIDE_LEFT ? RIGHT_HERO : LEFT_HERO;
            auto* targetEntity = _BATTLE_ENTITY.getEntity(targetId);
            
            if(targetEntity)
            {
                auto position = targetEntity->component<BattleComponent::Position>();
                auto tartgetIdentify = targetEntity->component<BattleComponent::Identify>();
                
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
                            if(id != 0 && MapHelper::getSide(id) != tartgetIdentify->side && MapHelper::getEntityType(id) == BattleConfig::ENTITY_SOLDIER)
                            {
                                current++;
                            }
                        }
                    }
                }
            }
        }
        return current >= count;
    }
    return false;
}
