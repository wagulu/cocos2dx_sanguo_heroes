//
//  CurveArrow.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/28/15.
//
//

#include "SkillArrow.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void SkillArrow::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    // play animation
    entityx::Entity* e = _BATTLE_ENTITY.getEntity(m_fromId);
    if(e)
    {
        BattleComponent::Position::Handle position = e->component<BattleComponent::Position>();
        BattleComponent::Identify::Handle identify = e->component<BattleComponent::Identify>();
        
        BattleConfig::SkillArrowType type = EnumParserSkillArrowType().getValue(*m_skillActionData->config["type"]->s);
        
        float speed = m_skillActionData->config["speed"]->d;
        int radius = -1;
        if(m_skillActionData->hasMember("radius")) radius = (int)m_skillActionData->config["radius"]->d;
        int range = -1;
        if(m_skillActionData->hasMember("range")) range = (int)m_skillActionData->config["range"]->d;
        std::string trigger = "";
        if(m_skillActionData->hasMember("trigger")) trigger = *m_skillActionData->config["trigger"]->s;
        int hp = -1;
        if(m_skillActionData->hasMember("hp")) hp = caclFormulaResult(*m_skillActionData->config["hp"]->s);
        bool follow = true;
        if(m_skillActionData->hasMember("follow")) follow = m_skillActionData->config["follow"]->b;
        bool destroy = true;
        if(m_skillActionData->hasMember("destroy")) destroy = m_skillActionData->config["destroy"]->b;
        
        BattleConfig::SkillArrowData arrowData(type, identify->side, identify->faceTo, speed, m_entityId, m_skillId, radius, range, hp, trigger);
        
        if(type == BattleConfig::ARROW_SKILL_CURVE)
        {
            arrowData.phase = m_skillActionData->config["phase"]->d;
            arrowData.frequency = m_skillActionData->config["frequency"]->d;
            arrowData.amplitude = m_skillActionData->config["amplitude"]->d;
        }
        
        _ENTITY_EVENT.emit<BattleEvent::AddSkillArrow>(m_effect->getEffect(*(m_skillActionData->config["animation"]->s)),
                                                       position->x, position->y, position->z, arrowData, follow, destroy);
    }
    
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
}

SkillArrow::~SkillArrow()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
}

void SkillArrow::receive(const BattleEvent::SkillArrowDestroyEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    remove();
}

void SkillArrow::receive(const BattleEvent::SkillArrowHitEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    auto position = _BATTLE_ENTITY.getEntity(m_fromId)->component<BattleComponent::Position>();
    for(auto target : e.targets)
    {
        BattleConfig::AttackData attackData(m_fromId, 0, 1, 0.2);
        attackData.setTargetId(target);
        attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
        if(MapHelper::getEntityType(target) == BattleConfig::ENTITY_SOLDIER)
        {
            if(m_skillActionData->hasMember("ignoreSoldier") && m_skillActionData->config["ignoreSoldier"]->b) break;
            
            if(m_skillActionData->hasMember("soldier_effect"))
            {
                bool reserse = false;
                auto* pTargetEntity = _BATTLE_ENTITY.getEntity(target);
                if (pTargetEntity)
                {
                    reserse = pTargetEntity->component<BattleComponent::Identify>()->faceTo == BattleConfig::FACE_TO_RIGHT;
                }
                
                BattleConfig::EffectGroup* group = m_effect->getEffect(*(m_skillActionData->config["soldier_effect"]->s));
                _ENTITY_EVENT.emit<BattleEvent::AddEffect>(group, reserse, position->x, 0, position->z, 0, 0);
            }
        }
        else
        {
            if(m_skillActionData->hasMember("boss_effect"))
            {
                bool reserse = false;
                auto* pTargetEntity = _BATTLE_ENTITY.getEntity(target);
                if (pTargetEntity)
                {
                    reserse = pTargetEntity->component<BattleComponent::Identify>()->faceTo == BattleConfig::FACE_TO_RIGHT;
                }
                
                BattleConfig::EffectGroup* group = m_effect->getEffect(*(m_skillActionData->config["boss_effect"]->s));
                _ENTITY_EVENT.emit<BattleEvent::AddEffect>(group, reserse, position->x, 0, position->z, 0, 0);
            }
        }
        
        attackData.damagePosition.x = position->x;
        attackData.damagePosition.y = 0;
        attackData.damagePosition.z = position->z;
        
        attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH;
        attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV;
        attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
        
        _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    }
}
