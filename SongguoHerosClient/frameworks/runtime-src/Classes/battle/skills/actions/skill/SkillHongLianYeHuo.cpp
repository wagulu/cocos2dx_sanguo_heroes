//
//  SkillHongLianYeHuo.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/19/15.
//
//

#include "SkillHongLianYeHuo.h"
#include "BattleWorld.h"
#include "ColorTween.h"

SkillHongLianYeHuo::~SkillHongLianYeHuo()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void SkillHongLianYeHuo::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL && e.attackData.fromId == m_fromId)
    {
        auto animation = e.targetEntity.component<BattleComponent::Animation>();
        
        animation->m_tweenQueue.pushTween(TweenQueue::TARGET::G,
                                          TweenQueue::OPERATION::IN,
                                          new SimpleTween_linear(0.7, 0.3, 1.0),
                                          0);
        
        animation->m_tweenQueue.pushTween(TweenQueue::TARGET::B,
                                          TweenQueue::OPERATION::IN,
                                          new SimpleTween_linear(0.7, 0.3, 1.0),
                                          0);
    }
}

void SkillHongLianYeHuo::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* entity = _BATTLE_ENTITY.getEntity(m_fromId);
    if(entity)
    {
        auto identify = (*entity).component<BattleComponent::Identify>();
        int targetId = identify->side == BattleConfig::SIDE_LEFT ? RIGHT_HERO : LEFT_HERO;
        auto targetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if(targetEntity)
        {
            auto position = (*targetEntity).component<BattleComponent::Position>();
            _ENTITY_MANAGER.get(m_entityId).assign<BattleComponent::SkillTileTarget>(MapHelper::getTileX(position->x), MapHelper::getTileZ(position->z));
            _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
        }
        else
        {
            remove();
        }
    }
    else
    {
        remove();
    }
}
