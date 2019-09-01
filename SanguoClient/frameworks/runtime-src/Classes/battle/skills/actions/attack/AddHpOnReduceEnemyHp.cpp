//
//  AddHpOnReduceEnemyHp.cpp
//  sanguoGame
//
//  Created by fuchenhao on 8/26/15.
//
//

#include "AddHpOnReduceEnemyHp.h"
#include "BattleWorld.h"

AddHpOnReduceEnemyHp::~AddHpOnReduceEnemyHp()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void AddHpOnReduceEnemyHp::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL
        && e.attackData.skillEntityId == m_entityId
        && e.attackData.finalHpChanged < 0)
    {
        m_delay = m_skillActionData->config["delay"]->d;
        
        float hurt = abs(e.attackData.finalHpChanged);
        float factor = caclFormulaResult(*m_skillActionData->config["factorFormula"]->s);
        m_hpAdded = ceil(hurt * factor);
        
        if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
        {
            _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
        }
    }
}

void AddHpOnReduceEnemyHp::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (m_hpAdded > 0)
    {
        m_delay -= dt;
        if (m_delay <= 0)
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(m_fromId);
            if (pTargetEntity)
            {
                auto targetProperty = pTargetEntity->component<BattleComponent::Property>();
                
                float maxHP = targetProperty->getOriginal(BattleConfig::Property::HP);
                float curHP = targetProperty->get(BattleConfig::Property::HP);
                float hp = curHP + m_hpAdded;
                if (hp > maxHP) hp = maxHP;
                if (hp < 0) hp = 0;
                targetProperty->set(BattleConfig::Property::HP, hp);
                
                //            float actualHpChanged = hp - curHP;
                
                _EFFECT_CREATOR.createHPIncreaseEffect(*pTargetEntity, *pTargetEntity, m_hpAdded, false);
            }
            
            remove();
        }
    }
}

void AddHpOnReduceEnemyHp::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}
