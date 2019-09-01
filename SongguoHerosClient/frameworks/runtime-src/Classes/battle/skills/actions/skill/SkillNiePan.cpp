//
//  SkillNiePan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/27/15.
//
//

#include "SkillNiePan.h"
#include "BattleWorld.h"

SkillNiePan::~SkillNiePan()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

//remove();

void SkillNiePan::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getTargetId() == m_fromId)
    {
        auto* entity = _BATTLE_ENTITY.getEntity(m_fromId);
        if(!entity) return;
        
        auto property = (*entity).component<BattleComponent::Property>();
        float curHp = property->get(BattleConfig::Property::HP);
        if(curHp <= 0)
        {
            //TO-DO play effect
            //move camera
            //set to Idle?
            
            //复活后HP百分比：0.3+abilityLevel*0.01
            //复活后MP增益百分比：min(1,0.3+rounddown(技能等级×1)/50)
            float maxHp = property->getOriginal(BattleConfig::Property::HP);
            float hp = (m_skillActionData->config["hpBase"]->d + m_skillData->level * m_skillActionData->config["hpFactor"]->d) * maxHp;
            property->set(BattleConfig::Property::HP, hp);
            
            float maxMp = property->getOriginal(BattleConfig::Property::MP);
            float mp = (min(m_skillActionData->config["mpMin"]->d, m_skillActionData->config["mpBase"]->d + m_skillData->level * m_skillActionData->config["mpFactor"]->d)) * maxMp;
            property->set(BattleConfig::Property::MP, mp);
            
            auto position = entity->component<BattleComponent::Position>();
            _HERO_MOVEMENT.moveTo(m_startX, position->y, position->z, (*entity));

            remove();
        }
    }
}

void SkillNiePan::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pEntity)
    {
        m_startX = pEntity->component<BattleComponent::Position>()->x;
        
        _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
    }
}
