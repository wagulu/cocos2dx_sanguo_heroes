//
//  WuYueHuaZhan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#include "SkillWuYueHuaZhan.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void SkillWuYueHuaZhan::fireAction(entityx::EntityManager &es)
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
        int baseHp = -1;
        if(m_skillActionData->hasMember("baseHp")) baseHp = (int)m_skillActionData->config["baseHp"]->d;
        int stepHp = 0;
        if(m_skillActionData->hasMember("stepHp")) stepHp = (int)m_skillActionData->config["stepHp"]->d;
        std::string trigger = "";
        if(m_skillActionData->hasMember("trigger")) trigger = *m_skillActionData->config["trigger"]->s;
        
        
        auto hp = caclFormulaResult(*m_skillActionData->config["hp"]->s);
        
        auto effect = m_effect->getEffect(*(m_skillActionData->config["animation"]->s));
        for(int i = -2; i <= 2; i++)
        {
            BattleConfig::SkillArrowData arrowData(type, identify->side, identify->faceTo, speed, m_entityId, m_skillId,
                                                   radius, range, hp, trigger);
            
            arrowData.breakDistance = m_skillActionData->config["breakDistance"]->d;
            arrowData.targetZ = position->z + i * GRID_HEIGHT * 4;
            
            bool follow = i == 0;
            _ENTITY_EVENT.emit<BattleEvent::AddSkillArrow>(effect, position->x, position->y, position->z, arrowData, follow);
        }
    }
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
    //_ENTITY_EVENT.subscribe<BattleEvent::SkillArrowOutEvent>(*this);
}

SkillWuYueHuaZhan::~SkillWuYueHuaZhan()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
    //_ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowOutEvent>(*this);
}

void SkillWuYueHuaZhan::receive(const BattleEvent::SkillArrowDestroyEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    m_total--;
    if(m_total <= 0) remove();
}

void SkillWuYueHuaZhan::receive(const BattleEvent::SkillArrowOutEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
}

void SkillWuYueHuaZhan::receive(const BattleEvent::SkillArrowHitEvent& e)
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
            //Soldier
            attackData.damageData.damageType = BattleConfig::DAMAGE_KILL;
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
