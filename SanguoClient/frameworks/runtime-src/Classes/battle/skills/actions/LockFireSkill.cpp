//
//  LockFireSkill.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/3/15.
//
//

#include "LockFireSkill.h"
#include "BattleWorld.h"

void LockFireSkill::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pFromEntity)
    {
        _ENTITY_EVENT.emit<BattleEvent::LockSkill>(MapHelper::getSide(m_fromId));
        
        if (m_skillActionData->hasMember("unlockDelay"))
        {
            m_unlockDelay = m_skillActionData->config["unlockDelay"]->d;
        }
        if (m_skillActionData->hasMember("unlockTrigger"))
        {
            m_pUnlockTrigger = m_skillActionData->config["unlockTrigger"]->s;
        }
        m_unlockRequired = true;
    }
}

void LockFireSkill::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (m_unlockDelay > 0)
    {
        m_unlockDelay -= dt;
        if (m_unlockDelay <= 0)
        {
            unlockFireSkill();
            remove();
        }
    }
}

void LockFireSkill::onTriggerFired(const std::string& trigger)
{
    if (m_pUnlockTrigger && *m_pUnlockTrigger == trigger)
    {
        m_unlockDelay = 1.7;
    }
}

void LockFireSkill::unlockFireSkill()
{
    if(m_unlockRequired)
    {
        _ENTITY_EVENT.emit<BattleEvent::UnlockSkill>(MapHelper::getSide(m_fromId));
        m_unlockRequired = false;
    }
}

LockFireSkill::~LockFireSkill()
{
    unlockFireSkill();
}
