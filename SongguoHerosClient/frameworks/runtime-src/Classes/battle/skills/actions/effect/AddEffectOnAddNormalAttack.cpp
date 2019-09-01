//
//  AddEffectOnAddNormalAttack.cpp
//  sanguoGame
//
//  Created by fuchenhao on 8/27/15.
//
//

#include "AddEffectOnAddNormalAttack.h"
#include "BattleWorld.h"

AddEffectOnAddNormalAttack::~AddEffectOnAddNormalAttack()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::AddAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::AddAttack>(*this);
    }
}

void AddEffectOnAddNormalAttack::receive(const BattleEvent::AddAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::NORMAL && e.attackData.fromId == m_fromId)
    {
        auto* pEntity = _BATTLE_ENTITY.getEntity(m_fromId);
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(e.attackData.getTargetId());
        if (pEntity && pTargetEntity)
        {
            auto position = pEntity->component<BattleComponent::Position>();
            auto targetPos = pTargetEntity->component<BattleComponent::Position>();
            
            bool reserse = (targetPos->x > position->x);
            auto* group = m_effect->getEffect(m_effectName);
            _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(group, reserse, *pEntity, m_effectDelay, m_effectDuration, m_fromId);
        }
    }
}

void AddEffectOnAddNormalAttack::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (elapse >= m_duration)
    {
        remove();
    }
}

void AddEffectOnAddNormalAttack::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::AddAttack>(*this);
    
    m_duration = caclFormulaResult(*m_skillActionData->config["durationFormula"]->s);
    m_effectName = *m_skillActionData->config["effect"]->s;
    m_effectDelay = m_skillActionData->config["effectDelay"]->d;
    m_effectDuration = m_skillActionData->config["effectDuration"]->d;
}
