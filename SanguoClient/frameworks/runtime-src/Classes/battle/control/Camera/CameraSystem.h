//
//  CameraSystem.h
//  SG
//
//  Created by fuchenhao on 3/3/15.
//
//

#ifndef __CAMERA_SYSTEM_H__
#define __CAMERA_SYSTEM_H__

#include "../../../entityx/entityx.h"

#include "BattleConfig.h"
#include "BattleComponent.h"
#include "BattleWorld.h"

#include "CameraManager.h"

class CameraSystem : public entityx::System<CameraSystem>
{
private:
    float m_fromX = CameraManager::INIT_CAMERA_X;
    float m_fromY = CameraManager::INIT_CAMERA_Y;
    float m_fromZ = CameraManager::INIT_CAMERA_Z;
    float m_fromRotate = CameraManager::INIT_CAMERA_ROTATE;
    float m_fromRotate2d = CameraManager::INIT_CAMERA_ROTATE_2D;
    float m_fromRotateY = CameraManager::INIT_CAMERA_ROTATE_Y;
    
    void setCameraFromInfo(BattleComponent::Position::Handle camPos);
    void setCameraFromInfoX(BattleComponent::Position::Handle camPos);
    void setCameraFromInfoY(BattleComponent::Position::Handle camPos);
    void setCameraFromInfoZ(BattleComponent::Position::Handle camPos);
    void setCameraFromInfoR3d(BattleComponent::Position::Handle camPos);
    void setCameraFromInfoR2d(BattleComponent::Position::Handle camPos);
    void setCameraFromInfoRY(BattleComponent::Position::Handle camPos);
    
public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};


#endif
