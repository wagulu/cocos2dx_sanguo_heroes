//
//  TriggerOnSkillAttack.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/31/15.
//
//

#include "TriggerOnSkillAttack.h"
#include "BattleWorld.h"

TriggerOnSkillAttack::~TriggerOnSkillAttack()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void TriggerOnSkillAttack::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL && e.attackData.skillEntityId == m_entityId)
    {
        int targetId = e.attackData.getTargetId();
        auto* entity = _BATTLE_ENTITY.getEntity(targetId);
        
        if(entity)
        {
            if(MapHelper::getEntityType(targetId) == BattleConfig::ENTITY_HERO)
            {
                if(m_skillActionData->hasMember("boss"))
                {
                    _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, *m_skillActionData->config["boss"]->s);
                }
            }
            else
            {
                if(m_skillActionData->hasMember("soldier"))
                {
                    _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, *m_skillActionData->config["soldier"]->s);
                }
            }
        }
    }
}

void TriggerOnSkillAttack::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}
