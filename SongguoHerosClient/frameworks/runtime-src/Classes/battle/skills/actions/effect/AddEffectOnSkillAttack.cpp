//
//  AddEffectOnSkillAttack.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/29/15.
//
//

#include "AddEffectOnSkillAttack.h"
#include "BattleWorld.h"

AddEffectOnSkillAttack::~AddEffectOnSkillAttack()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

//soldier//boss
void AddEffectOnSkillAttack::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL && e.attackData.skillEntityId == m_entityId)
    {
        int targetId = e.attackData.getTargetId();
        auto* entity = _BATTLE_ENTITY.getEntity(targetId);
        
        if(entity)
        {
            auto identify = (*entity).component<BattleComponent::Identify>();
            
            if(MapHelper::getEntityType(targetId) == BattleConfig::ENTITY_HERO)
            {
                if(m_skillActionData->hasMember("boss"))
                {
                    BattleConfig::EffectGroup* group = m_effect->getEffect(*(m_skillActionData->config["boss"]->s));
                    bool reserse = identify->faceTo == BattleConfig::FACE_TO_RIGHT;
                    _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(group, reserse, *entity, 0, 0, m_fromId);
                }
            }
            else
            {
                if(m_skillActionData->hasMember("soldier"))
                {
                    BattleConfig::EffectGroup* group = m_effect->getEffect(*(m_skillActionData->config["soldier"]->s));
                    bool reserse = identify->faceTo == BattleConfig::FACE_TO_RIGHT;
                    _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(group, reserse, *entity, 0, 0, m_fromId);
                }
            }
        }
    }
}

void AddEffectOnSkillAttack::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}
