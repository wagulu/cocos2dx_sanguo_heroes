//
//  ShakeCamera.cpp
//  Game
//
//  Created by fuchenhao on 15-3-16.
//
//

#include "ShakeCamera.h"
#include "BattleWorld.h"

void ShakeCamera::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    BattleConfig::CameraShakeData shakeData(0.5f, 20.0f, 5, 2, 0.0f);
    
    if (m_skillActionData->config["duration"] != NULL)
    {
        shakeData.duration = m_skillActionData->config["duration"]->d;
    }
    if (m_skillActionData->config["amplitude"] != NULL)
    {
        shakeData.amplitude = m_skillActionData->config["amplitude"]->d;
    }
    if (m_skillActionData->config["mode"] != NULL)
    {
        shakeData.mode = m_skillActionData->config["mode"]->d;
    }
    if (m_skillActionData->config["interval"] != NULL)
    {
        shakeData.interval = m_skillActionData->config["interval"]->d;
    }
    if (m_skillActionData->config["delay"] != NULL)
    {
        shakeData.delay = m_skillActionData->config["delay"]->d;
    }
    
    shakeData.side = m_side;
    
    _ENTITY_EVENT.emit<BattleEvent::CameraShakeEvent>(shakeData);
    
    remove();
}
