//
//  DelayTrigger.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/17/15.
//
//

#include "DelayTrigger.h"
#include "BattleWorld.h"

void DelayTrigger::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    if (m_skillActionData->hasMember("delay"))
    {
        m_delay = m_skillActionData->config["delay"]->d;
    }
    else if (m_skillActionData->hasMember("delayFormula"))
    {
        m_delay = caclFormulaResult(*m_skillActionData->config["delayFormula"]->s);
    }
    else
    {
        remove();
    }
}

void DelayTrigger::update(entityx::EntityManager &es, float elapse, float dt)
{
    m_elapse += dt;
    if(m_elapse >= m_delay)
    {
        _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, *m_skillActionData->config["trigger"]->s);
        
        remove();
    }
}
