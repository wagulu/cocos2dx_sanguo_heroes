//
//  SkillRelease.cpp
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#include "SkillRelease.h"
#include "BattleWorld.h"

void SkillRelease::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::FireSkill>(*this);
}

SkillRelease::~SkillRelease()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::FireSkill>(*this);
}

void SkillRelease::receive(const BattleEvent::FireSkill& e)
{
    if (e.fromId != m_heroId) return;
    
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_heroId);
    if (pEntity == nullptr) return;
    
    auto gasGather = pEntity->component<BattleComponent::GasGather>();
    if (gasGather->skills.size() > m_data->value && e.pSkillData->id == gasGather->skills[m_data->value]->id)
    {
        m_ok = true;
    }
}

