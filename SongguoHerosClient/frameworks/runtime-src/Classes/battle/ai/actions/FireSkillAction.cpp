//
//  FireSkillAction.cpp
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#include "FireSkillAction.h"

#include "../AIAction.h"
#include "BattleWorld.h"

void FireSkillAction::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::FireSkill>(*this);
}

void FireSkillAction::tryAction()
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity == nullptr) return;
    
    auto gasGather = pEntity->component<BattleComponent::GasGather>();
    if (gasGather->skills.size() > m_data->value)
    {
        _ENTITY_EVENT.emit<BattleEvent::PreFireSkill>(gasGather->skills[m_data->value]->id, m_heroId);
    }
}

FireSkillAction::~FireSkillAction()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::FireSkill>(*this);
}

void FireSkillAction::receive(const BattleEvent::FireSkill& e)
{
    if (e.fromId != m_heroId) return;
    
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity == nullptr) return;
    
    auto gasGather = pEntity->component<BattleComponent::GasGather>();
    if (gasGather->skills.size() > m_data->value && e.pSkillData->id == gasGather->skills[m_data->value]->id)
    {
        m_completed = true;
    }
};
