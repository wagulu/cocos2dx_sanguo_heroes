//
//  NoSelfSkillFired.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/19/15.
//
//

#include "NoSelfSkillFired.h"
#include "BattleWorld.h"

void NoSelfSkillFired::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::CompleteSkill>(*this);
    m_ok = true;
}

NoSelfSkillFired::~NoSelfSkillFired()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::CompleteSkill>(*this);
}

void NoSelfSkillFired::receive(const BattleEvent::CompleteSkill& e)
{
    if (e.fromId == m_heroId)
    {
        m_ok = false;
    }
}

