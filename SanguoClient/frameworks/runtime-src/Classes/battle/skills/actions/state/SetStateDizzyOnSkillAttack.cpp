//
//  SetStateDizzyOnSkillAttack.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/23/15.
//
//

#include "SetStateDizzyOnSkillAttack.h"
#include "BattleWorld.h"

SetStateDizzyOnSkillAttack::~SetStateDizzyOnSkillAttack()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void SetStateDizzyOnSkillAttack::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL && e.attackData.skillEntityId == m_entityId)
    {
        setTargetDizzy(e.attackData.getTargetId());
    }
}

void SetStateDizzyOnSkillAttack::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}
