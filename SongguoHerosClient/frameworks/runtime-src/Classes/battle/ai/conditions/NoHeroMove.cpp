//
//  NoHeroMove.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#include "NoHeroMove.h"
#include "BattleWorld.h"

bool NoHeroMove::isOk()
{
    auto* entity = _BATTLE_ENTITY.getEntity(m_heroId);
    if(entity)
    {
        auto objectStateMachine = (*entity).component<BattleComponent::ObjectStateMachine>();
        int objectStateCode = objectStateMachine->getCurrentStateCode();
        return (objectStateCode != BattleConfig::GENERAL_RUN);
    }
    return false;
}
