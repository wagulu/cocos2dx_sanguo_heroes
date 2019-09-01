//
//  ShowMoveEffect.cpp
//  Game
//
//  Created by zhaikaiyu on 3/9/15.
//
//

#include "SkillFenShenNuZhan.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void SkillFenShenNuZhan::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    entityx::Entity* e = _BATTLE_ENTITY.getEntity(m_fromId);
    if (e)
    {
        BattleComponent::Position::Handle position = e->component<BattleComponent::Position>();
        BattleComponent::Identify::Handle identify = e->component<BattleComponent::Identify>();
        BattleConfig::SkillArrowType type = EnumParserSkillArrowType().getValue(*m_skillActionData->config["type"]->s);
        
        float speed = m_skillActionData->config["speed"]->d;
        int radius = (m_skillActionData->hasMember("radius") ? (int)m_skillActionData->config["radius"]->d : -1);
        int range = (m_skillActionData->hasMember("range") ? (int)m_skillActionData->config["range"]->d : -1);
        int hp = -1;
        if(m_skillActionData->hasMember("hp")) hp = caclFormulaResult(*m_skillActionData->config["hp"]->s);
        std::string trigger = (m_skillActionData->hasMember("trigger") ? *m_skillActionData->config["trigger"]->s : "");

        BattleConfig::SkillArrowData arrowData(type, identify->side, identify->faceTo, speed, m_entityId, m_skillId, radius, range, hp, trigger);
        _ENTITY_EVENT.emit<BattleEvent::AddSkillArrow>(m_effect->getEffect(*(m_skillActionData->config["animation"]->s)), position->x, position->y, position->z, arrowData);
        
        _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowHitEvent>(*this);
        _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
        _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowOutEvent>(*this);
        _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowOutOfHpEvent>(*this);
    }
}

SkillFenShenNuZhan::~SkillFenShenNuZhan()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::SkillArrowHitEvent>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowHitEvent>(*this);
    }
    if (_ENTITY_EVENT.hasReceive<BattleEvent::SkillArrowDestroyEvent>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
    }
    if (_ENTITY_EVENT.hasReceive<BattleEvent::SkillArrowOutEvent>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowOutEvent>(*this);
    }
    if (_ENTITY_EVENT.hasReceive<BattleEvent::SkillArrowOutOfHpEvent>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowOutOfHpEvent>(*this);
    }
}

void SkillFenShenNuZhan::receive(const BattleEvent::SkillArrowDestroyEvent& e)
{
    if (e.id != m_entityId) return;
    
    remove();
}

void SkillFenShenNuZhan::receive(const BattleEvent::SkillArrowOutEvent& e)
{
    if (e.id != m_entityId) return;
    
    fireFinalHitAtPosition(e.z, e.x);
}

void SkillFenShenNuZhan::receive(const BattleEvent::SkillArrowOutOfHpEvent& e)
{
    if (e.id != m_entityId) return;
    
    fireFinalHitAtPosition(e.z, e.x);
}

void SkillFenShenNuZhan::receive(const BattleEvent::SkillArrowHitEvent& e)
{
    if (e.id != m_entityId) return;
    
    auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pFromEntity == nullptr) return;
    
    auto position = pFromEntity->component<BattleComponent::Position>();
    
    for (int targetId : e.targets)
    {
        BattleConfig::AttackData attackData(m_fromId);
        attackData.setTargetId(targetId);
        attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
        
        attackData.damagePosition.x = position->x;
        attackData.damagePosition.y = 0;
        attackData.damagePosition.z = position->z;
        attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH;
        attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV;
        attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;

        _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
        
        if (MapHelper::isHero(targetId))
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity != nullptr)
            {
                auto targetPos = pTargetEntity->component<BattleComponent::Position>();
                
                fireFinalHitAtPosition(targetPos->z, targetPos->x);
            }
        }
    }
}

void SkillFenShenNuZhan::fireFinalHitAtPosition(float z, float x)
{
    if (!m_finalHitFired)
    {
        m_finalHitFired = true;
        
        _ENTITY_MANAGER.get(m_entityId).assign<BattleComponent::SkillTileTarget>(MapHelper::getTileX(x), MapHelper::getTileZ(z));
        
        _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "finalHit");
    }
}
