//
//  IntervalTrigger.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/13/15.
//
//

#include "IntervalTrigger.h"

void IntervalTrigger::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    m_trigger = m_skillActionData->config["trigger"]->s;
    m_interval = m_skillActionData->config["interval"]->d;
    
    if(m_skillActionData->hasMember("times"))
    {
        m_times = (int)m_skillActionData->config["times"]->d;
    }
    else if(m_skillActionData->hasMember("duration"))
    {
        m_times = floor(caclFormulaResult(*m_skillActionData->config["duration"]->s) / m_interval);
    }
    else
    {
        remove();
        return;
    }
    
    m_times--;
    _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, *m_trigger);
}

void IntervalTrigger::update(entityx::EntityManager &es, float elapse, float dt)
{
    m_elapse += dt;
    if(m_elapse >= m_interval)
    {
        m_elapse -= m_interval;
        
        m_times--;
        _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, *m_trigger);
        
        if(m_times <= 0) remove();
    }
}
