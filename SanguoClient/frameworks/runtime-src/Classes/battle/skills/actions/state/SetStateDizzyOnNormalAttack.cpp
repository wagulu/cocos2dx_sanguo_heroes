//
//  SetStateDizzyOnNormalAttack.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/23/15.
//
//

#include "SetStateDizzyOnNormalAttack.h"
#include "BattleWorld.h"

SetStateDizzyOnNormalAttack::~SetStateDizzyOnNormalAttack()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void SetStateDizzyOnNormalAttack::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::NORMAL && e.attackData.fromId == m_fromId)
    {
        setTargetDizzy(e.attackData.getTargetId());
    }
}

void SetStateDizzyOnNormalAttack::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}
