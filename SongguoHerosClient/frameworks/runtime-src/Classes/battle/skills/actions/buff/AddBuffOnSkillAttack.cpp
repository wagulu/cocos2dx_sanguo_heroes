//
//  AddBuffOnSkillAttack.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#include "AddBuffOnSkillAttack.h"
#include "BattleWorld.h"

AddBuffOnSkillAttack::~AddBuffOnSkillAttack()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void AddBuffOnSkillAttack::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL && e.attackData.skillEntityId == m_entityId)
    {
        addBuffToTarget(e.attackData.getTargetId());
        
        if (m_onlyOnce)
        {
            remove();
        }
    }
}

void AddBuffOnSkillAttack::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    if (m_skillActionData->hasMember("once"))
    {
        m_onlyOnce = m_skillActionData->config["once"]->b;
    }
    else
    {
        m_onlyOnce = false;
    }
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}
