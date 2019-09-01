//
//  SkillChuanSuo.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#include "SkillChuanSuo.h"
#include "BattleWorld.h"
#include "StateSoldierChangeToGhost.h"

using namespace entityx;

void SkillChuanSuo::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    // play animation
    entityx::Entity* e = _BATTLE_ENTITY.getEntity(m_fromId);
    if(e)
    {
        int range = -1;
        if(m_skillActionData->hasMember("range")) range = (int)m_skillActionData->config["range"]->d;
        BattleComponent::Position::Handle position = e->component<BattleComponent::Position>();
        BattleComponent::Identify::Handle identify = e->component<BattleComponent::Identify>();
        BattleConfig::SkillArrowType type = BattleConfig::SkillArrowType::ARROW_SKILL_LINE;
        float speed = -m_skillActionData->config["speed"]->d;
        BattleConfig::SkillArrowData arrowData(type, identify->side, identify->faceTo, speed, m_entityId, m_skillId, 0, range);
        int xBias = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -1) * (range + 1) * GRID_WIDTH;
        
        auto* targetEntity = _BATTLE_ENTITY.getEntity(identify->side == BattleConfig::SIDE_LEFT ? RIGHT_HERO : LEFT_HERO);
        bool follow = false;
        if(targetEntity)
        {
            int targetTileX = MapHelper::getTileX((*targetEntity).component<BattleComponent::Position>()->x);
            int myTileX = MapHelper::getTileX((*e).component<BattleComponent::Position>()->x);
            follow = (abs(targetTileX - myTileX) >= range);
        }
        _ENTITY_EVENT.emit<BattleEvent::AddSkillArrow>(m_effect->getEffect(*(m_skillActionData->config["animation"]->s)), position->x + (float)xBias, position->y, position->z, arrowData, follow, false);
    }
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
}

SkillChuanSuo::~SkillChuanSuo()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
    //_ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowOutEvent>(*this);
}

void SkillChuanSuo::receive(const BattleEvent::SkillArrowDestroyEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    remove();
}

void SkillChuanSuo::receive(const BattleEvent::SkillArrowHitEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    for(auto target : e.targets)
    {
        if(MapHelper::getEntityType(target) == BattleConfig::ENTITY_HERO)
        {
            BattleConfig::HeroDizzyConfig dizzyConfig = *m_skillData->pConfig->dizzy->getConfig(*m_skillActionData->config["dizzyConfig"]->s);
            
            auto* pDizzyActionSkill = dizzyConfig.findAction("skill_chuan_suo");
            if (pDizzyActionSkill != nullptr)
            {
                auto* targetEntity = _BATTLE_ENTITY.getEntity(target);
                
                if(targetEntity)
                {
                    BattleConfig::HeroDizzyData dizzyData;
                    dizzyData.skillEntityId = m_entityId;
                    
                    _ENTITY_EVENT.emit(BattleEvent::StrategyHeroDizzy(*targetEntity, dizzyData, dizzyConfig));
                    _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "hitHero");
                    
                    _ENTITY_EVENT.emit<BattleEvent::CameraFollowEntity>(e.entity);
                }
                remove();
            }
        }
    }
}










