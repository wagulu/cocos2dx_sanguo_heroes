//
//  DizzyActionKnockBack.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/28/15.
//
//

#include "DizzyActionKnockBack.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

DizzyActionKnockBack::~DizzyActionKnockBack()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void DizzyActionKnockBack::enter(entityx::Entity& entity)
{
    m_entity = entity;
    m_heroId = m_entity.component<BattleComponent::Identify>()->id;
    m_knockBackTile = (int)m_pActionConfig->config["knockBackTile"].d;
    
    if (m_pActionConfig->hasMember("knockBackTrigger"))
    {
        m_knockBackTrigger = *m_pActionConfig->config["knockBackTrigger"].s;
        
        _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
    }
    else
    {
        knockBackHero(m_entity);
    }
}

void DizzyActionKnockBack::exit(entityx::Entity& entity)
{
}

void DizzyActionKnockBack::update(entityx::Entity& entity, float dt)
{
    
}

void DizzyActionKnockBack::receive(const BattleEvent::HurtByAttack& e)
{
    if (e.attackData.getAttackType() == BattleConfig::AttackType::SKILL
        && e.attackData.skillEntityId == m_pDizzyData->skillEntityId
        && e.attackData.getTargetId() == m_heroId
        && e.attackData.damageData.triggerSkillActionOnAttack != nullptr
        && *e.attackData.damageData.triggerSkillActionOnAttack == m_knockBackTrigger
        && m_entity.valid())
    {
        knockBackHero(m_entity);
    }
}

void DizzyActionKnockBack::knockBackHero(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto positon = entity.component<BattleComponent::Position>();
    
    float toX = positon->x + (identify->faceTo == BattleConfig::FaceTo::FACE_TO_LEFT ? m_knockBackTile : -m_knockBackTile) * GRID_WIDTH;
    
    _HERO_MOVEMENT.moveTo(toX, positon->y, positon->z, entity, HeroMovement::MoveIgnoreType::IGNORE_ALL_SOLDIER);
}
