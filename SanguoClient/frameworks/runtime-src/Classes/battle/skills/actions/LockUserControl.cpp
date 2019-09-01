//
//  LockUserControl.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/8/15.
//
//

#include "LockUserControl.h"
#include "BattleWorld.h"

void LockUserControl::fireAction(entityx::EntityManager &es)
{
    m_elapse = 0;
    SkillAction::fireAction(es);
    
    _ENTITY_EVENT.subscribe<BattleEvent::SetCameraModeForLockEvent>(*this);
    
    auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pFromEntity)
    {
        if (m_skillActionData->hasMember("unlockDelay"))
        {
            m_unlockDelay = m_skillActionData->config["unlockDelay"]->d;
        }
        if (m_skillActionData->hasMember("unlockTrigger"))
        {
            m_pUnlockTrigger = m_skillActionData->config["unlockTrigger"]->s;
        }
        m_unlockRequired = true;
        
        if (_CAMERA_MANAGER.isCameraSinus())
        {
            m_preCameraMode = CameraManager::CAMERA_SINUS;
        }
        else if(_CAMERA_MANAGER.isCameraFollow())
        {
            m_preCameraMode = CameraManager::CAMERA_FOLLOW_CAN_BREAK;
            m_cameraFollowTargetId = _CAMERA_MANAGER.getCameraFollowTargetId();
        }
        
//        if (m_skillActionData->hasMember("noBack"))
//        {
//            m_autoBack = false; //m_skillActionData->config["noBack"]->d;
//        }
        
        if (m_side == BattleConfig::SIDE_LEFT)
        {
            _ENTITY_EVENT.emit<BattleEvent::CameraDisableShakeEvent>(true);
        }
        _CAMERA_MANAGER.setCameraUserControlLocked(true);
    }
    
//    m_timeTween = new SimpleTween_linear(0.5, 1.0f, 3.0f);
//    m_timeTween2 = new SimpleTween_linear(2.5, 3.0f, 1.0f);
}

void LockUserControl::update(entityx::EntityManager &es, float elapse, float dt)
{
    m_elapse += dt;
    if (m_unlockDelay > 0)
    {
        m_unlockDelay -= dt;
        if (m_unlockDelay <= 0)
        {
            unlockUserControl();
            remove();
        }
    }
}

void LockUserControl::onTriggerFired(const std::string& trigger)
{
    if (m_pUnlockTrigger && *m_pUnlockTrigger == trigger)
    {
        m_unlockDelay = 1.5;
    }
}

void LockUserControl::unlockUserControl()
{
    if(m_unlockRequired)
    {
        if (m_side == BattleConfig::SIDE_LEFT)
        {
            _ENTITY_EVENT.emit<BattleEvent::CameraDisableShakeEvent>(false);
        }
        _CAMERA_MANAGER.setCameraUserControlLocked(false);
        
        m_unlockRequired = false;
        
        if (m_preCameraMode == CameraManager::CAMERA_SINUS)
        {
            _CAMERA_MANAGER.initSmartFollowSolider(true);
        }
        else if(m_preCameraMode == CameraManager::CAMERA_FOLLOW_CAN_BREAK && m_autoBack)
        {
            _ENTITY_EVENT.emit<BattleEvent::CompleteSkill>(m_fromId);
            
            if (m_cameraFollowTargetId == LEFT_HERO || m_cameraFollowTargetId == RIGHT_HERO)
            {
                auto* pEntity = _BATTLE_ENTITY.getEntity(m_cameraFollowTargetId);
                if (pEntity)
                {
                    _ENTITY_EVENT.emit<BattleEvent::CameraFollowEntity>(*pEntity);
                }
            }
        }
    }
}


void LockUserControl::receive(const BattleEvent::SetCameraModeForLockEvent& e)
{
    if(e.sinus)
    {
        m_preCameraMode = CameraManager::CAMERA_SINUS;
    }
    else
    {
        m_preCameraMode = CameraManager::CAMERA_FOLLOW_CAN_BREAK;
        m_cameraFollowTargetId = e.roleID;
    }
}


void LockUserControl::resetCamera()
{
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                 0.2, 0,
                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                 CameraManager::EASE_OUT, 0);
    
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D,
                                                                 0.2, 0,
                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                 CameraManager::EASE_OUT, 0);
    
//    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
//                                                               0.2, 0,
//                                                               CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                               CameraManager::EASE_OUT, -1);
//    
//    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
//                                                               0.2, 0,
//                                                               CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                               CameraManager::EASE_OUT, -1);
//    
//    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
//                                                                 0.2, 0,
//                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                                 CameraManager::EASE_OUT, -1);
}


LockUserControl::~LockUserControl()
{
    unlockUserControl();
    resetCamera();
    
    delete m_timeTween;
    
    if (_ENTITY_EVENT.hasReceive<BattleEvent::SetCameraModeForLockEvent>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::SetCameraModeForLockEvent>(*this);
    }
}
