//
//  SkillNuJi.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/30/15.
//
//

#include "SkillNuJi.h"
#include "BattleWorld.h"
#include "cocos2d.h"

SkillNuJi::~SkillNuJi()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void SkillNuJi::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.fromId == m_fromId && e.attackData.getAttackType() == BattleConfig::AttackType::NORMAL)
    {
        
        auto* entity = _BATTLE_ENTITY.getEntity(m_fromId);
        if(!entity) return;
        
        int targetId = e.attackData.getTargetId();
        auto* targetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if(!targetEntity) return;
        
        int myTileX = MapHelper::getTileX((*entity).component<BattleComponent::Position>()->x);
        int targetTileX = MapHelper::getTileX((*targetEntity).component<BattleComponent::Position>()->x);
        
        auto identify = (*entity).component<BattleComponent::Identify>();
        if((identify->side == BattleConfig::SIDE_LEFT && myTileX < targetTileX) || (identify->side == BattleConfig::SIDE_RIGHT && myTileX > targetTileX))
        {
            _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "attackRect");
        }
        else
        {
            _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "attackRectBack");
        }
        
    }
}

void SkillNuJi::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    this->m_endTime = this->caclFormulaResult(*this->m_skillActionData->config["durationFormula"]->s);
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}

