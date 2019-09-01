//
//  SkillSystem.h
//  Game
//
//  Created by fuchenhao on 2/9/15.
//
//


#include "SkillSystem.h"
#include "BattleWorld.h"
#include "AudioManager.h"
#include "AICommonHelper.h"

SkillSystem::~SkillSystem()
{
}

void SkillSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::FireSkill>(*this);
    events.subscribe<BattleEvent::FireNegativeSkill>(*this);
    events.subscribe<BattleEvent::CancelSkill>(*this);
    events.subscribe<BattleEvent::TriggerSkillAction>(*this);
    events.subscribe<BattleEvent::BattlePause>(*this);
    events.subscribe<BattleEvent::BattleResume>(*this);
}

void SkillSystem::receive(const BattleEvent::BattlePause& e)
{
    m_isPausing = true;
}

void SkillSystem::receive(const BattleEvent::BattleResume& e)
{
    m_isPausing = false;
}

void SkillSystem::receive(const BattleEvent::FireSkill& e)
{
    auto entity = _ENTITY_MANAGER.create();
    auto* skillObj = SkillManager::getInstance()->createSkill(e.pSkillData, e.fromId, entity.id());
    entity.assign<BattleComponent::Skill>(skillObj, e.fromId);
    
    _CAMERA_MANAGER.setCameraCauseID(entity.id().id());
}

void SkillSystem::receive(const BattleEvent::FireNegativeSkill& e)
{
    auto entity = _ENTITY_MANAGER.create();
    auto* skillObj = SkillManager::getInstance()->createSkill(e.pSkillData, e.fromId, entity.id());
    entity.assign<BattleComponent::Skill>(skillObj, e.fromId);
}

void SkillSystem::receive(const BattleEvent::CancelSkill& e)
{
    auto identify = e.entity.component<BattleComponent::Identify>();
    
    BattleComponent::Skill::Handle skill;
    for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(skill))
    {
        if (LEFT_HERO == identify->id)
        {
            _CAMERA_MANAGER.setCameraUserControlLocked(false);
        }
        
        bool breakSucceed = false;
        if (skill->fromId == identify->id)
        {
            auto* pSkillData = skill->skill->getSkillData();
            if (e.forceCancel)
            {
                breakSucceed = true;
            }
            else if (pSkillData->pConfig->type != BattleConfig::SkillType::NEGATIVE)
            {
                if (pSkillData->canBreak || isDead(e.entity))
                {
                    breakSucceed = true;
                }
            }
        }
        
        if (breakSucceed)
        {
            CCLOG("skill cancelled of hero:%d name:%s", identify->id, skill->skill->getSkillData()->name.c_str());
         
            for (int soundId : skill->m_playingSoundPool)
            {
                AudioManager::getInstance()->stopEffect(soundId);
            }
            skill->m_playingSoundPool.clear();
            
            BattleComponent::Effect::Handle effect;
            for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(effect))
            {
                if (effect->fromId == identify->id)
                {
                    entity.destroy();
                }
            }
            
            BattleComponent::Attack::Handle attack;
            for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(attack))
            {
                if (attack->attackData.fromId == identify->id)
                {
                    entity.destroy();
                }
            }
            
            entity.destroy();
        }
    }
}

void SkillSystem::receive(const BattleEvent::TriggerSkillAction& e)
{
    if(_ENTITY_MANAGER.valid(e.id))
    {
        auto entity = _ENTITY_MANAGER.get(e.id);
        BattleComponent::Skill::Handle skill = entity.component<BattleComponent::Skill>();
        skill->skill->triggerAction(e.trigger);
    }
}

void SkillSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::Skill::Handle skill;

    for (entityx::Entity entity : es.entities_with_components(skill))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            if(skill->skill->shouldRemove())
            {
                entity.destroy();
            }
            else
            {
                skill->elapse += dt;
                skill->skill->update(es, skill->elapse, dt);
            }
        }
    }
}

