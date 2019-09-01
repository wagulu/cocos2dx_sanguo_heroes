//
//  AfterAnySkillRelease.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/8/15.
//
//

#include "AfterAnySkillRelease.h"
#include "BattleWorld.h"

void AfterAnySkillRelease::configure()
{
    _ENTITY_EVENT.subscribe<BattleEvent::FireSkill>(*this);
}

AfterAnySkillRelease::~AfterAnySkillRelease()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::FireSkill>(*this);
}

void AfterAnySkillRelease::receive(const BattleEvent::FireSkill& e)
{
    m_elapse = 0;
    m_fired = true;
}

bool AfterAnySkillRelease::isOk()
{
    if(m_fired && shouldCheck())
    {
        m_elapse++;
        
        if(m_elapse >= m_data->value)
        {
            return true;
        }
    }
    return false;
}

