//
//  SkillZhanHun.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/27/15.
//
//

#include "SkillZhanHun.h"
#include "BattleWorld.h"
#include "cocos2d.h"

SkillZhanHun::~SkillZhanHun()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

//remove();

void SkillZhanHun::receive(const BattleEvent::HurtByAttack& e)
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
            
            //max(0.3,min(0.7,rounddown(abilityLevel/100)))
            float maxHp = property->getOriginal(BattleConfig::Property::HP);
            float hp = (m_skillActionData->config["hpBase"]->d + m_skillData->level * m_skillActionData->config["hpFactor"]->d) * maxHp;
            property->set(BattleConfig::Property::HP, hp);
            
            //存活时间：min(10,5+rounddown(技能等级×0.5)/5) 致死攻击，对自己
            float lifeTime = min(m_skillActionData->config["lifeMin"]->d, m_skillActionData->config["lifeBase"]->d + m_skillData->level * m_skillActionData->config["lifeFactor"]->d);
            
            BattleConfig::AttackData attackData(m_fromId, lifeTime);
            attackData.setTargetId(m_fromId);
            attackData.damageData.damageType = BattleConfig::DAMAGE_KILL;
            
            _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
            
            //send event to add buff
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("reborn");
            
            remove();
        }
    }
}

void SkillZhanHun::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}
