//
//  NoOppSkillFired.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/10/15.
//
//

#include "NoOppSkillFired.h"
#include "BattleWorld.h"

void NoOppSkillFired::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::CompleteSkill>(*this);
    m_ok = true;
}

NoOppSkillFired::~NoOppSkillFired()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::CompleteSkill>(*this);
}

void NoOppSkillFired::receive(const BattleEvent::CompleteSkill& e)
{
    if (e.fromId != m_heroId)
    {
        m_ok = false;
    }
}

