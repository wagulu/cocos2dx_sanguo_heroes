//
//  SkillShunXiQianLi.cpp
//  SGGame
//
//  Created by fuchenhao on 4/28/15.
//
//

#include "SkillShunXiQianLi.h"
#include "BattleWorld.h"

//using namespace entityx;

void SkillShunXiQianLi::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);

    auto* heroEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (heroEntity)
    {
        _ENTITY_EVENT.emit<BattleEvent::StrategyHeroDash>(*heroEntity, *(m_skillActionData->config["animation"]->s), m_skillActionData->config["dur"]->d, m_skillActionData->config["topSpeed"]->d);
        
        BattleConfig::CameraShakeData shakeData(0.5f, 10.0f, 5, 2, 0.2f);
        _ENTITY_EVENT.emit<BattleEvent::CameraShakeEvent>(shakeData);
        
        auto* pEffectGroup = m_effect->getEffect(*m_skillActionData->config["effect"]->s);
        
        float duration = 10.0f;
        if (m_skillActionData->config["dur"])
        {
            duration = m_skillActionData->config["dur"]->d;
        }

        if (pEffectGroup != nullptr) {
                auto identify = heroEntity->component<BattleComponent::Identify>();
                bool reserse = identify->faceTo == BattleConfig::FACE_TO_RIGHT;
                _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(pEffectGroup, reserse, *heroEntity, 0.1, duration);
        }
        
        _ENTITY_EVENT.emit<BattleEvent::CameraFollowEntity>(*heroEntity);

        if(m_side == BattleConfig::SIDE_LEFT)
        {
            
            
            
            float r2D = 0.0f;
            if (m_skillActionData->config["r2D"])
            {
                r2D = m_skillActionData->config["r2D"]->d;
            }
            
            bool Rback = false;
            if (m_skillActionData->config["Rback"])
            {
                Rback = true;
            }
            
            float rY = 0.0f;
            if (m_skillActionData->config["rY"])
            {
                rY = m_skillActionData->config["rY"]->d;
            }
            
            _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D - r2D,
                                                                         duration, 0,
                                                                         CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                         CameraManager::EASE_OUT, 0);
            
            
            //        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(INIT_CAMERA_ROTATE_Y - rY*2,
            //                                                                    duration * 0.5, 0,
            //                                                                    CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
            //                                                                    CameraManager::LINEAR, 0);
            //
            //        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(INIT_CAMERA_ROTATE_Y,
            //                                                                    duration * 0.5, 0,
            //                                                                    CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
            //                                                                    CameraManager::EASE_OUT, 0, false);
            
            if (Rback)
            {
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
                
                
                //            _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(INIT_CAMERA_ROTATE_Y,
                //                                                                        1.0, 0,
                //                                                                        CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                //                                                                        CameraManager::EASE_OUT, 0, false);
            }
            else
            {
                //            _BATTLE_STAGE.showBGColor(duration, 166, 0, 0);
            }
            
            
            
            float colorR = 0.0f;
            if (m_skillActionData->config["colorR"])
            {
                colorR = m_skillActionData->config["colorR"]->d;
            }
            
            float colorG = 0.0f;
            if (m_skillActionData->config["colorG"])
            {
                colorG = m_skillActionData->config["colorG"]->d;
            }
            
            float colorB = 0.0f;
            if (m_skillActionData->config["colorB"])
            {
                colorB = m_skillActionData->config["colorB"]->d;
            }
            
            if (colorR > 0 || colorG > 0 || colorB > 0)
            {
                _BATTLE_STAGE.showBGColor(duration, colorR, colorG, colorB);
            }
            
            
            //        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D - 5,
            //                                                                     duration * 0.8, 0,
            //                                                                     CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
            //                                                                     CameraManager::LINEAR, 0, false);
            
            //        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D,
            //                                                                     duration * 0.8, 0,
            //                                                                     CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
            //                                                                     CameraManager::EASE_OUT, 0, false);
            
            //        _BATTLE_STAGE.showBackgroundMask(m_showDuration, m_showColor);
        }
 
    }
    
    remove();
}
