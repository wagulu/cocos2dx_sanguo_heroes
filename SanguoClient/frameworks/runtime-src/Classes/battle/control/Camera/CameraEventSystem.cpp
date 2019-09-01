//
//  CameraEventSystem.cpp
//  SG
//
//  Created by fuchenhao on 15-3-3.
//
//

#include "CameraEventSystem.h"

void CameraEventSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::CameraMoveToEvent>(*this);
    events.subscribe<BattleEvent::CameraMoveToWithAniEvent>(*this);
    
    events.subscribe<BattleEvent::CameraMoveToWithAniXEvent>(*this);
    events.subscribe<BattleEvent::CameraMoveToWithAniYEvent>(*this);
    events.subscribe<BattleEvent::CameraMoveToWithAniZEvent>(*this);
    events.subscribe<BattleEvent::CameraMoveToWithAniR3dEvent>(*this);
    events.subscribe<BattleEvent::CameraMoveToWithAniR2dEvent>(*this);
    events.subscribe<BattleEvent::CameraMoveToWithAniRYEvent>(*this);
    
    events.subscribe<BattleEvent::CameraFollowEntity>(*this);
    events.subscribe<BattleEvent::CameraTriggerFollowEntity>(*this);
    events.subscribe<BattleEvent::CameraShakeEvent>(*this);
    events.subscribe<BattleEvent::CameraClearShakeEvent>(*this);
    events.subscribe<BattleEvent::CameraDisableShakeEvent>(*this);
    events.subscribe<BattleEvent::CameraQueueClearEvent>(*this);
}

void CameraEventSystem::receive(const BattleEvent::CameraMoveToEvent& e)
{
    _CAMERA_MANAGER.setCameraX(e.toX);
    _CAMERA_MANAGER.setCameraZ(e.toZ);
    _CAMERA_MANAGER.setCameraYFree(e.toY);
}

void CameraEventSystem::receive(const BattleEvent::CameraMoveToWithAniEvent& e)
{
    // game over || user control
    if (e.causeId == -2 || e.causeId == -1) {
        _CAMERA_MANAGER.setCameraCauseID(e.causeId);
    }
    
    uint64_t cameraC = _CAMERA_MANAGER.getCameraCauseID();
    if(cameraC == -2 && e.causeId != -2) return;

    if (cameraC <= 0 || cameraC == e.causeId)
    {
        if (e.force)
        {
            _CAMERA_MANAGER.clearCameraQueue();
        }
        
        _CAMERA_MANAGER.addCameraQueueToX(e.toX, e.time, e.delay, e.nextCameraMode, e.tweenMode);
        _CAMERA_MANAGER.addCameraQueueToY(e.toY, e.time, e.delay, e.nextCameraMode, e.tweenMode, e.backDelay);
        _CAMERA_MANAGER.addCameraQueueToZ(e.toZ, e.time, e.delay, e.nextCameraMode, e.tweenMode, e.backDelay);
        _CAMERA_MANAGER.addCameraQueueToR3d(e.rotate, e.time, e.delay, e.nextCameraMode, e.tweenMode, e.backDelay);
        
        _CAMERA_MANAGER.setCameraModeX(e.curCameraMode);
        _CAMERA_MANAGER.setCameraModeY(e.curCameraMode);
        _CAMERA_MANAGER.setCameraModeZ(e.curCameraMode);
        _CAMERA_MANAGER.setCameraModeR3d(e.curCameraMode);
    }
}


void CameraEventSystem::receive(const BattleEvent::CameraMoveToWithAniXEvent& e)
{
    // game over || user control
    if (e.causeId == -2 || e.causeId == -1 || e.causeId == -3) {
        _CAMERA_MANAGER.setCameraCauseID(e.causeId);
    }
    
    uint64_t cameraC = _CAMERA_MANAGER.getCameraCauseID();
    if(cameraC == -2 && e.causeId != -2) return;
    
    if (cameraC <= 0 || cameraC == e.causeId)
    {
        if (e.force)
        {
            _CAMERA_MANAGER.clearCameraQueue();
        }
        //        _CAMERA_MANAGER.addCameraQueueTo(e.toX, e.toY, e.toZ, e.rotate, e.time, e.delay, e.nextCameraMode, e.tweenMode);
        
        _CAMERA_MANAGER.addCameraQueueToX(e.toX, e.time, e.delay, e.nextCameraMode, e.tweenMode);
        
        _CAMERA_MANAGER.setCameraModeX(e.curCameraMode);
    }
}

void CameraEventSystem::receive(const BattleEvent::CameraMoveToWithAniYEvent& e)
{
    if (e.force)
    {
        _CAMERA_MANAGER.clearCameraQueueY();
    }

    _CAMERA_MANAGER.addCameraQueueToY(e.toY, e.time, e.delay, e.nextCameraMode, e.tweenMode);
    
    _CAMERA_MANAGER.setCameraModeY(e.curCameraMode);
}

void CameraEventSystem::receive(const BattleEvent::CameraMoveToWithAniZEvent& e)
{
    if (e.force)
    {
        _CAMERA_MANAGER.clearCameraQueueZ();
    }
    
    _CAMERA_MANAGER.addCameraQueueToZ(e.toZ, e.time, e.delay, e.nextCameraMode, e.tweenMode);
    
    _CAMERA_MANAGER.setCameraModeZ(e.curCameraMode);
}

void CameraEventSystem::receive(const BattleEvent::CameraMoveToWithAniR3dEvent& e)
{
    if (e.force)
    {
        _CAMERA_MANAGER.clearCameraQueueR3d();
    }
    
    _CAMERA_MANAGER.addCameraQueueToR3d(e.toR3d, e.time, e.delay, e.nextCameraMode, e.tweenMode);
    
    _CAMERA_MANAGER.setCameraModeR3d(e.curCameraMode);
}

void CameraEventSystem::receive(const BattleEvent::CameraMoveToWithAniR2dEvent& e)
{
    if (e.force)
    {
        _CAMERA_MANAGER.clearCameraQueueR2d();
    }
    
    _CAMERA_MANAGER.addCameraQueueToR2d(e.toR2d, e.time, e.delay, e.nextCameraMode, e.tweenMode);
    
    _CAMERA_MANAGER.setCameraModeR2d(e.curCameraMode);
}

void CameraEventSystem::receive(const BattleEvent::CameraMoveToWithAniRYEvent& e)
{
    if (e.force)
    {
        _CAMERA_MANAGER.clearCameraQueueRY();
    }
    
    _CAMERA_MANAGER.addCameraQueueToRY(e.toRY, e.time, e.delay, e.nextCameraMode, e.tweenMode);
    
    _CAMERA_MANAGER.setCameraModeRY(e.curCameraMode);
}


void CameraEventSystem::receive(const BattleEvent::CameraFollowEntity& e)
{
    _CAMERA_MANAGER.setCameraFollowTarget(e.tarEntity);

    if (e.setState)
    {
        _CAMERA_MANAGER.setCameraModeFollow(true, e.canBreak);
    }
}

void CameraEventSystem::receive(const BattleEvent::CameraTriggerFollowEntity& e)
{
    _CAMERA_MANAGER.setCameraModeFollow(e.following);
}

void CameraEventSystem::receive(const BattleEvent::CameraQueueClearEvent& e)
{
    _CAMERA_MANAGER.clearCameraQueue();
}


void CameraEventSystem::receive(const BattleEvent::CameraDisableShakeEvent& e)
{
    m_shakeDisable = e.diable;
}

void CameraEventSystem::receive(const BattleEvent::CameraShakeEvent& e)
{
    if (e.cameraShakeData.side == BattleConfig::SIDE_RIGHT && m_shakeDisable) return;

//    CCLOG("========= Camera Shake!!!!");
    entityx::Entity shakeCtrl = _ENTITY_MANAGER.create();
    shakeCtrl.assign<BattleComponent::ShakeCom>(e.cameraShakeData.duration, e.cameraShakeData.amplitude,
                                                e.cameraShakeData.mode, e.cameraShakeData.interval, e.cameraShakeData.delay, e.cameraShakeData.damping);
}

void CameraEventSystem::receive(const BattleEvent::CameraClearShakeEvent& e)
{
    BattleComponent::ShakeCom::Handle shake;
    for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(shake))
    {
        entity.destroy();
    }
}


// nothing to do
void CameraEventSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
    
}
