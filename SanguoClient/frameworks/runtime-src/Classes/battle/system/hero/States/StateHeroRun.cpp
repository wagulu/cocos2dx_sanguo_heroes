//
//  StateHeroRun.cpp
//  Game
//
//  Created by fuchenhao on 15-2-27.
//
//

#include "StateHeroRun.h"
#include "HeroSystem.h"
#include "../../AICommonHelper.h"
#include "BattleEntity.h"
#include "AudioSystem.h"
#include "AudioManager.h"

StateHeroRun::StateHeroRun()
{

}

StateHeroRun::~StateHeroRun()
{
    
}

void StateHeroRun::enter(entityx::Entity& entity)
{
    m_isMainHero = MapHelper::isMainHero(entity.component<BattleComponent::Identify>()->id);
    
    std::string eventName = AUDIOEVENTS_HERO_MOVE;
    std::string triggerName = "hero"; //getHeroSoldierAudioTriggerName(entity);
    BattleEvent::BattleSound soundEvent(eventName, triggerName, entityx::Entity());
    _ENTITY_EVENT.emit(soundEvent);

    m_audioId = *soundEvent.pSoundId;
}

void StateHeroRun::exit(entityx::Entity& entity)
{
    AudioManager::getInstance()->stopEffect(m_audioId);
}

int StateHeroRun::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    if (_TARGET_FINDER.findHeroFrontNearbyTarget(entity) > 0)
    {
        _RETURN_STATE_RESULT(ATTACK_ENEMY);
    }
//    else if (!m_isMainHero && !_TARGET_FINDER.findEnemyHeroOnSameLine(entity))
//    {
//        _RETURN_STATE_RESULT(NEED_CHANGE_LINE);
//    }
    else
    {
        auto animation = entity.component<BattleComponent::Animation>();
        auto identify = entity.component<BattleComponent::Identify>();
        auto direction = entity.component<BattleComponent::Direction>();
        direction->x = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -1);
        direction->y = 0;
        direction->z = 0;
        
        bool block = _HERO_MOVEMENT.move(dt, entity);
        if (block)
        {
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE);
        }
        else
        {
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_RUN);
        }
    }
    
    return m_result;
}

