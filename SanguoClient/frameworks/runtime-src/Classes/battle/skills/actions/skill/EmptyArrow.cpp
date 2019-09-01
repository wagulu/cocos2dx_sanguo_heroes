//
//  EmptyArrow.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/22/15.
//
//

#include "EmptyArrow.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void EmptyArrow::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    // play animation
    entityx::Entity* e = _BATTLE_ENTITY.getEntity(m_fromId);
    if(e)
    {
        BattleComponent::Position::Handle position = e->component<BattleComponent::Position>();
        BattleComponent::Identify::Handle identify = e->component<BattleComponent::Identify>();
        BattleConfig::SkillArrowType type = BattleConfig::SkillArrowType::ARROW_SKILL_LINE;
        float speed = m_skillActionData->config["speed"]->d;
        int range = -1;
        if(m_skillActionData->hasMember("range")) range = (int)m_skillActionData->config["range"]->d;
        BattleConfig::SkillArrowData arrowData(type, identify->side, identify->faceTo, speed, m_entityId, m_skillId,
                                               0, range);
        _ENTITY_EVENT.emit<BattleEvent::AddSkillArrow>(m_effect->getEffect(*(m_skillActionData->config["animation"]->s)), position->x, position->y, position->z, arrowData, true);
    }
    
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
}

EmptyArrow::~EmptyArrow()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::SkillArrowHitEvent>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::SkillArrowDestroyEvent>(*this);
}

void EmptyArrow::receive(const BattleEvent::SkillArrowDestroyEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    remove();
}

void EmptyArrow::receive(const BattleEvent::SkillArrowHitEvent& e)
{
    if (e.skillId != m_skillId || e.id != m_entityId)
    {
        return;
    }
    
    for(auto target : e.targets)
    {
        if(MapHelper::getEntityType(target) == BattleConfig::ENTITY_HERO)
        {
            _ENTITY_EVENT.emit<BattleEvent::CameraTriggerFollowEntity>(false);
        }
    }
}


