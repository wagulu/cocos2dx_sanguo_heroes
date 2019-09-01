//
//  MoveCameraToTile.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#include "MoveCameraToTile.h"
#include "BattleWorld.h"

void MoveCameraToTile::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto skillTileTarget = es.get(m_entityId).component<BattleComponent::SkillTileTarget>();
    if (skillTileTarget.valid())
    {
        float time = m_skillActionData->config["time"]->d;
        float delay = m_skillActionData->config["delay"]->d;
        float x = MapHelper::getCenterX(skillTileTarget->tileX, false);
        float y = CameraManager::INIT_CAMERA_Y;
        float z = CameraManager::INIT_CAMERA_Z;
        
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(x, z, y,
                                                                  time, delay,
                                                                  CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                  CameraManager::INIT_CAMERA_ROTATE, CameraManager::EASE_OUT, m_entityId.id());
    }
    
    remove();
}
