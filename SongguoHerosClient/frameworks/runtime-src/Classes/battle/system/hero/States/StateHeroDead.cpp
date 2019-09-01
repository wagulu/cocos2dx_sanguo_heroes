//
//  StateHeroDead.cpp
//  Game
//
//  Created by fuchenhao on 2/28/15.
//
//

#include "StateHeroDead.h"
#include "HeroSystem.h"
#include "../../AICommonHelper.h"
#include "AudioSystem.h"
#include "BattleWorld.h"

StateHeroDead::StateHeroDead(const BattleConfig::AttackData& attackData) : m_attackBringToDeath(attackData)
{
    m_killedBySkill = (attackData.getAttackType() == BattleConfig::AttackType::SKILL);
}

StateHeroDead::~StateHeroDead()
{
}

void StateHeroDead::enter(entityx::Entity& entity)
{
    auto objectStateMachine = entity.component<BattleComponent::ObjectStateMachine>();
    objectStateMachine->isDead = true;
    
    _ENTITY_EVENT.emit<BattleEvent::CancelSkill>(entity);
    
    if (m_killedBySkill)
    {
        _ENTITY_EVENT.emit<BattleEvent::PlayHeroDeadEffect>();
    }
    
    auto position = entity.component<BattleComponent::Position>();
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(position->x, CameraManager::INIT_CAMERA_Z, CameraManager::INIT_CAMERA_Y,
                                                              0.05, 0,
                                                              CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                              CameraManager::INIT_CAMERA_ROTATE, CameraManager::EASE_OUT, -2);
    
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                0.05, 0,
                                                                CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                CameraManager::EASE_OUT, 0);
    
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D,
                                                                 0.05, 0,
                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                 CameraManager::EASE_OUT, 0);
}

void StateHeroDead::exit(entityx::Entity& entity)
{
}

int StateHeroDead::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    if (m_subState == STATE_WAITING_FLY_TEXT_COMPLETE)
    {
        if (!entity.has_component<BattleComponent::FlyTextEffect>())
        {
            auto animation = entity.component<BattleComponent::Animation>();
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_DEATH, false);
            
            playSound(entity);

            m_subState = STATE_WAITING_DEAD_ANIM;
        }
    }
    else if (m_subState == STATE_WAITING_DEAD_ANIM)
    {
        auto animation = entity.component<BattleComponent::Animation>();
        if (animation->battleAnimation->isComplete())
        {
            _BATTLE_ENTITY.removeEntity(entity);
            entity.destroy();
            
            _ENTITY_EVENT.emit<BattleEvent::StateHeroDeadDone>(entity);
            
            _RETURN_STATE_RESULT(DONE);
        }
    }
    
    return m_result;
}

void StateHeroDead::playSound(entityx::Entity& entity)
{
    bool isWomen = isHeroWoman(entity);
    std::string eventName = AUDIOEVENTS_HERO_DEAD_MAN;
    if(isWomen)
    {
        eventName = AUDIOEVENTS_HERO_DEAD_WOMAN;
    }
    
    std::string triggerName = "hero";// getHeroSoldierAudioTriggerName(entity);
    BattleEvent::BattleSound event(eventName, triggerName, entity);
    _ENTITY_EVENT.emit(event);
}
