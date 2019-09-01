//
//  SkillOk.cpp
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#include "SkillOk.h"
#include "BattleWorld.h"

bool SkillOk::isOk()
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity != nullptr)
    {
        auto gasGather = pEntity->component<BattleComponent::GasGather>();
        
        if (gasGather->skills.size() > m_data->value)
        {
            return gasGather->skills[m_data->value]->isReady;
        }
    }
    return false;
}
