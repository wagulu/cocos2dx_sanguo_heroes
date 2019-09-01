//
//  PauseBattle.cpp
//  Game
//
//  Created by fuchenhao on 3/2/15.
//
//

#include "PauseBattle.h"
#include "BattleWorld.h"

void PauseBattle::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pEntity != nullptr && MapHelper::getSide(m_fromId) == BattleConfig::Side::SIDE_LEFT)
    {
        _BATTLE_STAGE.pauseBattle();//TO-DO change to event
        
        if (pEntity->has_component<BattleComponent::ActiveEntity>())
        {
            pEntity->component<BattleComponent::ActiveEntity>().remove();
        }
        pEntity->assign<BattleComponent::ActiveEntity>();
        
        BattleComponent::Skill::Handle skill;
        for (auto entity : es.entities_with_components(skill))
        {
            if (entity.id() == m_entityId)
            {
                entity.assign<BattleComponent::ActiveEntity>();
            }
        }
        
        m_duration = m_skillActionData->config["duration"]->d;
        m_resumeRequired = true;
    }
}

void PauseBattle::update(entityx::EntityManager &es, float elapse, float dt)
{
    m_duration -= dt;
    if (m_duration <= 0)
    {
        resumeBattle();
        remove();
    }
}

void PauseBattle::resumeBattle()
{
    if (m_resumeRequired)
    {
        _BATTLE_STAGE.resumeBattle();//TO-DO change to event
        m_resumeRequired = false;
    }
}

PauseBattle::~PauseBattle()
{
    resumeBattle();
}
