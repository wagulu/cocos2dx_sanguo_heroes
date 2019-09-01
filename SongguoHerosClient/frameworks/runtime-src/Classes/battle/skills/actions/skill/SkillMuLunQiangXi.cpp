//
//  SkillMuLunQiangXi.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/20/15.
//
//

#include "SkillMuLunQiangXi.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void SkillMuLunQiangXi::fireAction(entityx::EntityManager &es)
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
        //entityx::Entity::Id id,
        //int skillId,
        int radius = -1;
        if(m_skillActionData->hasMember("radius")) radius = (int)m_skillActionData->config["radius"]->d;
        int range = -1;
        if(m_skillActionData->hasMember("range")) range = (int)m_skillActionData->config["range"]->d;
        int hp = -1;
        if(m_skillActionData->hasMember("hp")) hp = caclFormulaResult(*m_skillActionData->config["hp"]->s);
        std::string trigger = "";
        if(m_skillActionData->hasMember("trigger")) trigger = *m_skillActionData->config["trigger"]->s;
        
        BattleConfig::SkillArrowData arrowData(type, identify->side, identify->faceTo, speed, m_entityId, m_skillId,
                                               radius, range, hp, trigger);
        
        int myTileX = MapHelper::getTileX(position->x);
        int total = (int)m_skillActionData->config["number"]->d;
        int start, end;
        if(total % 2 == 0)
        {
            start = -total / 2 + 1;
            end = total / 2 + 1;
        }
        else
        {
            start = -(total - 1) / 2;
            end = (total + 1) / 2;
        }
        for(int i = start; i < end; i++)
        {
            if(MapHelper::validTileX(myTileX + i * 2))
            {
                _ENTITY_EVENT.emit<BattleEvent::AddSkillArrow>(m_effect->getEffect(*(m_skillActionData->config["animation"]->s)), (myTileX + i * 2) * GRID_WIDTH + HALF_GRID_WIDTH, position->y, STAGE_HEIGHT * GRID_HEIGHT - HALF_GRID_HEIGHT, arrowData, false);
            }
        }
    }
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
    //_ENTITY_EVENT.subscribe<BattleEvent::SkillArrowOutEvent>(*this);
}

SkillMuLunQiangXi::~SkillMuLunQiangXi()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
    //_ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowOutEvent>(*this);
}

void SkillMuLunQiangXi::receive(const BattleEvent::SkillArrowDestroyEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    remove();
}

void SkillMuLunQiangXi::receive(const BattleEvent::SkillArrowOutEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
}

void SkillMuLunQiangXi::receive(const BattleEvent::SkillArrowHitEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    if(!_BATTLE_ENTITY.getEntity(m_fromId))return;
    
    auto position = _BATTLE_ENTITY.getEntity(m_fromId)->component<BattleComponent::Position>();
    for(auto target : e.targets)
    {
        BattleConfig::AttackData attackData(m_fromId, 0, 1, 0.2);
        attackData.setTargetId(target);
        attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
        
        attackData.damagePosition.x = position->x;
        attackData.damagePosition.y = 0;
        attackData.damagePosition.z = position->z;
        attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH;
        attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV;
        attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
        
        _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    }
}
