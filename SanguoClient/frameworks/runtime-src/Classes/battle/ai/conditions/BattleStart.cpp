//
//  BattleStartCondition.cpp
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#include "BattleStart.h"
#include "BattleWorld.h"
#include "BattleSystem.h"

bool BattleStart::isOk()
{
    if (!m_ok)
    {
        auto& battleEntity = _BATTLE_ENTITY.getBattle();
        auto objectStateMachine = battleEntity.component<BattleComponent::ObjectStateMachine>();
        if (objectStateMachine->getCurrentStateCode() == BattleSystem::BattleState::START_FIGHT)
        {
            m_ok = true;
        }
    }
    return m_ok;
}
