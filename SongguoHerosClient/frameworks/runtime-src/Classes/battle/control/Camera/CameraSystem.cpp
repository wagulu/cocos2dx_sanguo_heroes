//
//  CameraSystem.cpp
//  SG
//
//  Created by fuchenhao on 3/3/15.
//
//

#include "CameraSystem.h"

void CameraSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::Camera::Handle   camera;
    BattleComponent::Position::Handle position;
    
    for (entityx::Entity entity : es.entities_with_components(camera, position))
    {

        // X
        switch(camera->cameraModeX)
        {
            case CameraManager::CAMERA_LOCKED:
                break;
                
            case CameraManager::CAMERA_SINUS:
                _CAMERA_MANAGER.smartFollowSolider();
                _CAMERA_MANAGER.updateCameraPos(position);
                break;
                
            case CameraManager::CAMERA_FREE:
                _CAMERA_MANAGER.updateCameraPos(position);
                break;

            case CameraManager::CAMERA_FOLLOW:
            case CameraManager::CAMERA_FOLLOW_CAN_BREAK:

                if (!camera->followData.followTarget.valid())
                {
                    _CAMERA_MANAGER.setCameraModeX(CameraManager::CAMERA_FREE);
                }
                else {
                    auto positionTar = camera->followData.followTarget.component<BattleComponent::Position>();
                    if(fabs(position->x - FOLLOW_DISTANCE - positionTar->x) > NEAR_DISTANCE)
                    {
                        float moveTime = 0.1f;

                        if (fabs(position->x - FOLLOW_DISTANCE - positionTar->x) < 50)
                        {
                            moveTime = 0.05f;
                        }
                        _CAMERA_MANAGER.moveCameraFollowX(camera->followData.followTarget, position);
                    }
                    else
                    {
                        _CAMERA_MANAGER.moveCameraFollowX(camera->followData.followTarget, position);
                    }
                }
                break;

            case CameraManager::CAMERA_MOVE:
            case CameraManager::CAMERA_ZOOMING:
                if(camera->cameraXQueue.size() > 0)
                {
                    BattleConfig::CameraActionData* action = camera->cameraXQueue[0];
                    if(action->delay > 0)
                    {
                        action->delay -= dt;

                        if (action->delay <= 0)
                        {
                            setCameraFromInfoX(position);
                        }
                    }
                    else
                    {
                        if (action->delay == 0) {
                            action->delay -= dt;
                            setCameraFromInfoX(position);
                        }
                        if (action->time == 0)
                        {
                            _CAMERA_MANAGER.setCameraByActionX(action->toData, position);
                            camera->cameraXQueue.erase(camera->cameraXQueue.begin());
                            delete action;
                            _CAMERA_MANAGER.setCameraModeX(action->nextCameraState);
                        }
                        else
                        {
                            action->elapse += dt;
                            if(action->elapse < action->time)
                            {
                                float tX = _CAMERA_MANAGER.calcTweenPos(action->elapse, m_fromX, (action->toData - m_fromX), action->time, action->tweenMode);
                                _CAMERA_MANAGER.setCameraByActionX(tX, position);
                            }
                            else
                            {
                                _CAMERA_MANAGER.setCameraByActionX(action->toData, position);
                                camera->cameraXQueue.erase(camera->cameraXQueue.begin());
                                delete action;
                                _CAMERA_MANAGER.setCameraModeX(action->nextCameraState);
                            }
                        }
                    }
                }
                else
                {
                    _CAMERA_MANAGER.setCameraModeX(CameraManager::CAMERA_FREE);
                }
                break;
        }
        
        
        // Y
        switch(camera->cameraModeY)
        {
            case CameraManager::CAMERA_MOVE:
            case CameraManager::CAMERA_ZOOMING:
                if(camera->cameraYQueue.size() > 0)
                {
                    BattleConfig::CameraActionData* action = camera->cameraYQueue[0];
                    if(action->delay > 0)
                    {
                        action->delay -= dt;
                        
                        if (action->delay <= 0)
                        {
                            setCameraFromInfoY(position);
                        }
                    }
                    else
                    {
                        if (action->delay == 0) {
                            action->delay -= dt;
                            setCameraFromInfoY(position);
                        }
                        if (action->time == 0)
                        {
                            _CAMERA_MANAGER.setCameraByActionY(action->toData, position);
                            camera->cameraYQueue.erase(camera->cameraYQueue.begin());
                            delete action;
                        }
                        else
                        {
                            action->elapse += dt;
                            if(action->elapse < action->time)
                            {
                                float tY = _CAMERA_MANAGER.calcTweenPos(action->elapse, m_fromY, (action->toData - m_fromY), action->time, action->tweenMode);
                                _CAMERA_MANAGER.setCameraByActionY(tY, position);
                            }
                            else
                            {
                                _CAMERA_MANAGER.setCameraByActionY(action->toData, position);
                                camera->cameraYQueue.erase(camera->cameraYQueue.begin());
                                delete action;
                                _CAMERA_MANAGER.setCameraModeY(action->nextCameraState);
                            }
                        }
                    }
                }
                else
                {
                    _CAMERA_MANAGER.setCameraModeY(CameraManager::CAMERA_FREE);
                }
                break;
                
            case CameraManager::CAMERA_FREE:
                if (!_CAMERA_MANAGER.isCameraYDefault())
                {
                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
                                                                               0.2, camera->backYDelay,
                                                                               CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                               CameraManager::EASE_OUT, 0);
                }
                break;
        }

        
        // Z
        switch(camera->cameraModeZ)
        {
            case CameraManager::CAMERA_MOVE:
            case CameraManager::CAMERA_ZOOMING:
                if(camera->cameraZQueue.size() > 0)
                {
                    BattleConfig::CameraActionData* action = camera->cameraZQueue[0];
                    if(action->delay > 0)
                    {
                        action->delay -= dt;
                        
                        if (action->delay <= 0)
                        {
                            setCameraFromInfoZ(position);
                        }
                    }
                    else
                    {
                        if (action->delay == 0) {
                            action->delay -= dt;
                            setCameraFromInfoZ(position);
                        }
                        if (action->time == 0)
                        {
                            _CAMERA_MANAGER.setCameraByActionZ(action->toData, position);
                            camera->cameraZQueue.erase(camera->cameraZQueue.begin());
                            delete action;
                        }
                        else
                        {
                            action->elapse += dt;
                            if(action->elapse < action->time)
                            {
                                float tZ = _CAMERA_MANAGER.calcTweenPos(action->elapse, m_fromZ, (action->toData - m_fromZ), action->time, action->tweenMode);
                                _CAMERA_MANAGER.setCameraByActionZ(tZ, position);
                            }
                            else
                            {
                                _CAMERA_MANAGER.setCameraByActionZ(action->toData, position);
                                camera->cameraZQueue.erase(camera->cameraZQueue.begin());
                                delete action;
                                _CAMERA_MANAGER.setCameraModeZ(action->nextCameraState);
                            }
                        }
                    }
                }
                else
                {
                    _CAMERA_MANAGER.setCameraModeZ(CameraManager::CAMERA_FREE);
                }
                break;
                
            case CameraManager::CAMERA_FREE:
                if (!_CAMERA_MANAGER.isCameraZDefault())
                {
                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                                               0.2, camera->backZDelay,
                                                                               CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                               CameraManager::EASE_OUT, 0);
                }
                break;
        }
        
        
        // R3d
        switch(camera->cameraModeR3d)
        {
            case CameraManager::CAMERA_MOVE:
            case CameraManager::CAMERA_ZOOMING:
                if(camera->cameraR3dQueue.size() > 0)
                {
                    BattleConfig::CameraActionData* action = camera->cameraR3dQueue[0];
                    if(action->delay > 0)
                    {
                        action->delay -= dt;
                        
                        if (action->delay <= 0)
                        {
                            setCameraFromInfoR3d(position);
                        }
                    }
                    else
                    {
                        if (action->delay == 0) {
                            action->delay -= dt;
                            setCameraFromInfoR3d(position);
                        }
                        if (action->time == 0)
                        {
                            _CAMERA_MANAGER.setCameraRotation3D(action->toData);
                            camera->cameraR3dQueue.erase(camera->cameraR3dQueue.begin());
                            delete action;
                        }
                        else
                        {
                            action->elapse += dt;
                            if(action->elapse < action->time)
                            {
                                float tR = _CAMERA_MANAGER.calcTweenPos(action->elapse, m_fromRotate, (action->toData - m_fromRotate), action->time, action->tweenMode);
                                _CAMERA_MANAGER.setCameraRotation3D(tR);
                            }
                            else
                            {
                                _CAMERA_MANAGER.setCameraRotation3D(action->toData);
                                camera->cameraR3dQueue.erase(camera->cameraR3dQueue.begin());
                                delete action;
                                _CAMERA_MANAGER.setCameraModeR3d(action->nextCameraState);
                            }
                        }
                    }
                }
                else
                {
                    _CAMERA_MANAGER.setCameraModeR3d(CameraManager::CAMERA_FREE);
                }
                break;
                
            case CameraManager::CAMERA_FREE:
                if (!_CAMERA_MANAGER.isCameraR3dDefault())
                {
                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                                 0.2, camera->backR3dDelay,
                                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                                 CameraManager::EASE_OUT, 0);
                }
                break;
        }

        
        // R2d
        switch(camera->cameraModeR2d)
        {
            case CameraManager::CAMERA_MOVE:
            case CameraManager::CAMERA_ZOOMING:
                if(camera->cameraR2dQueue.size() > 0)
                {
                    BattleConfig::CameraActionData* action = camera->cameraR2dQueue[0];
                    if(action->delay > 0)
                    {
                        action->delay -= dt;
                        
                        if (action->delay <= 0)
                        {
                            setCameraFromInfoR2d(position);
                        }
                    }
                    else
                    {
                        if (action->delay == 0) {
                            action->delay -= dt;
                            setCameraFromInfoR2d(position);
                        }
                        if (action->time == 0)
                        {
                            _CAMERA_MANAGER.setCameraRotation2D(action->toData);
                            camera->cameraR2dQueue.erase(camera->cameraR2dQueue.begin());
                            delete action;
                        }
                        else
                        {
                            action->elapse += dt;
                            if(action->elapse < action->time)
                            {
                                float tR = _CAMERA_MANAGER.calcTweenPos(action->elapse, m_fromRotate2d, (action->toData - m_fromRotate2d), action->time, action->tweenMode);
                                _CAMERA_MANAGER.setCameraRotation2D(tR);
                            }
                            else
                            {
                                _CAMERA_MANAGER.setCameraRotation2D(action->toData);
                                camera->cameraR2dQueue.erase(camera->cameraR2dQueue.begin());
                                delete action;
                                _CAMERA_MANAGER.setCameraModeR2d(action->nextCameraState);
                            }
                        }
                    }
                }
                else
                {
                    _CAMERA_MANAGER.setCameraModeR2d(CameraManager::CAMERA_FREE);
                }
                break;
                
            case CameraManager::CAMERA_FREE:
                if (!_CAMERA_MANAGER.isCameraR2dDefault())
                {
                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D,
                                                                                 0.2, camera->backR2dDelay,
                                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                                 CameraManager::EASE_OUT, 0);
                }
                break;
        }
        
        
        // RY
        switch(camera->cameraModeRY)
        {
            case CameraManager::CAMERA_MOVE:
            case CameraManager::CAMERA_ZOOMING:
                if(camera->cameraRYQueue.size() > 0)
                {
                    BattleConfig::CameraActionData* action = camera->cameraRYQueue[0];
                    if(action->delay > 0)
                    {
                        action->delay -= dt;
                        
                        if (action->delay <= 0)
                        {
                            setCameraFromInfoRY(position);
                        }
                    }
                    else
                    {
                        if (action->delay == 0) {
                            action->delay -= dt;
                            setCameraFromInfoRY(position);
                        }
                        if (action->time == 0)
                        {
                            _CAMERA_MANAGER.setCameraRotationY(action->toData);
                            camera->cameraRYQueue.erase(camera->cameraRYQueue.begin());
                            delete action;
                        }
                        else
                        {
                            action->elapse += dt;
                            if(action->elapse < action->time)
                            {
                                float tR = _CAMERA_MANAGER.calcTweenPos(action->elapse, m_fromRotateY, (action->toData - m_fromRotateY), action->time, action->tweenMode);
                                _CAMERA_MANAGER.setCameraRotationY(tR);
                            }
                            else
                            {
                                _CAMERA_MANAGER.setCameraRotationY(action->toData);
                                camera->cameraRYQueue.erase(camera->cameraRYQueue.begin());
                                delete action;
                                _CAMERA_MANAGER.setCameraModeRY(action->nextCameraState);
                            }
                        }
                    }
                }
                else
                {
                    _CAMERA_MANAGER.setCameraModeRY(CameraManager::CAMERA_FREE);
                }
                break;
                
            case CameraManager::CAMERA_FREE:
//                if (!_CAMERA_MANAGER.isCameraRYDefault())
//                {
//                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(INIT_CAMERA_ROTATE_Y,
//                                                                                 0.2, camera->backRYDelay,
//                                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                                                 CameraManager::EASE_OUT, 0);
//                }
                break;
        }
    }
}

void CameraSystem::setCameraFromInfo(BattleComponent::Position::Handle camPos)
{
    m_fromX = camPos->x;
    m_fromY = camPos->y;
    m_fromZ = camPos->z;
    m_fromRotate = _CAMERA_MANAGER.getCameraRotation3D();
    m_fromRotate2d = _CAMERA_MANAGER.getCameraRotation2D();
    m_fromRotateY = _CAMERA_MANAGER.getCameraRotationY();
}

void CameraSystem::setCameraFromInfoX(BattleComponent::Position::Handle camPos)
{
    m_fromX = camPos->x;
}

void CameraSystem::setCameraFromInfoY(BattleComponent::Position::Handle camPos)
{
    m_fromY = camPos->y;
}

void CameraSystem::setCameraFromInfoZ(BattleComponent::Position::Handle camPos)
{
    m_fromZ = camPos->z;
}

void CameraSystem::setCameraFromInfoR3d(BattleComponent::Position::Handle camPos)
{
    m_fromRotate = _CAMERA_MANAGER.getCameraRotation3D();
}

void CameraSystem::setCameraFromInfoR2d(BattleComponent::Position::Handle camPos)
{
    m_fromRotate2d = _CAMERA_MANAGER.getCameraRotation2D();
//    m_fromRotate2d = 0;
}

void CameraSystem::setCameraFromInfoRY(BattleComponent::Position::Handle camPos)
{
    m_fromRotateY = _CAMERA_MANAGER.getCameraRotationY();
}

