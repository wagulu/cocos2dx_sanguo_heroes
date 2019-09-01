//
//  MoveCameraToHero.cpp
//  SG
//
//  Created by fuchenhao on 15-3-6.
//
//

#include "MoveCameraToHero.h"
#include "BattleWorld.h"


void MoveCameraToHero::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    float t = 0.2f;
    if (m_skillActionData->config["dur"] != NULL)
    {
        t = m_skillActionData->config["dur"]->d;
    }
    
    int toSide = BattleConfig::SIDE_LEFT;
    if (m_skillActionData->config["tar"] != NULL)
    {
        toSide = m_skillActionData->config["tar"]->d;
    }
    
    float r = CameraManager::INIT_CAMERA_ROTATE;
    float y = CameraManager::INIT_CAMERA_Y;
    float z = CameraManager::INIT_CAMERA_Z;
    float backDelay = 1.0f;
    if (m_skillActionData->config["zoom"] != NULL)
    {
        if (m_skillActionData->config["r"] != NULL)
        {
            r = m_skillActionData->config["r"]->d;
        }
        else
        {
            r = -5.0f;
        }
        
        if (m_skillActionData->config["y"] != NULL)
        {
            y = m_skillActionData->config["y"]->d;
        }
        else
        {
            y = 130.0f;
        }
        
        if (m_skillActionData->config["z"] != NULL)
        {
            z = m_skillActionData->config["z"]->d;
        }
        else
        {
            z = -200.0f;
        }
        
        if (m_skillActionData->config["backDelay"] != NULL)
        {
            backDelay = m_skillActionData->config["backDelay"]->d;
        }
    }
    
    //        CCLOG("============ backDelay:%f", backDelay);
    
    bool mStep = true;
    if (m_skillActionData->config["S"] != NULL)
    {
        mStep = false;
    }
    
    float dtx = 0.0f;
    if (m_skillActionData->config["dtx"] != NULL)
    {
        dtx = m_skillActionData->config["dtx"]->d;
    }
    
    int a = CameraManager::EASE_OUT;
    if (m_skillActionData->config["A"] != NULL)
    {
        a = m_skillActionData->config["A"]->d;
    }
    
    //        CCLOG("============ MoveCameraToHero:%d", toSide);
    //        CCLOG("============ MoveCameraToHero:zoom: %d", z);
    
    int targetId;
    if(toSide == BattleConfig::SIDE_RIGHT)
    {
        if(m_side == BattleConfig::SIDE_LEFT)
        {
            targetId = RIGHT_HERO;
            dtx = -dtx;
        }
        else
        {
            targetId = LEFT_HERO;
        }
    }
    else
    {
        if(m_side == BattleConfig::SIDE_LEFT)
        {
            targetId = LEFT_HERO;
        }
        else
        {
            targetId = RIGHT_HERO;
            dtx = -dtx;
        }
    }
    
    int styleID = -1;
    if (m_skillActionData->config["style"] != NULL && m_fromId == LEFT_HERO)
    {
        styleID = m_skillActionData->config["style"]->d;
    }

    // duo jiang ...
    if (m_fromId == LEFT_HERO || m_fromId == RIGHT_HERO)
    {
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if (pTargetEntity != nullptr)
        {
            auto position = pTargetEntity->component<BattleComponent::Position>();
            
            float tarX = position->x + dtx;
            if (m_skillActionData->config["abX"] != NULL)
            {
                tarX = m_skillActionData->config["abX"]->d;
            }
            
            if (styleID == 1)
            {
                tarX += 50;
            }
            
//            if (styleID == 2)
//            {
//                tarX -= 190;
//            }
            
            if (styleID != 3)
            {
                if (mStep)
                {
                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(tarX, CONF_INIT_CAMERA_Z, CONF_INIT_CAMERA_Y,
                                                                              t * 0.2, 0,
                                                                              CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                              CameraManager::INIT_CAMERA_ROTATE, a, m_entityId.id());
                    
                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(tarX, z, y,
                                                                              t * 0.8, t * 0.8,
                                                                              CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                              r, a, m_entityId.id(), false, backDelay);
                }
                else
                {
                    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(tarX, z, y,
                                                                              t, 0,
                                                                              CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                              r, a, m_entityId.id(), true, backDelay);
                }
            }
            
            // wu yue hua zhan
            if (styleID == 1)
            {
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            0.3, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y - 8,
                                                                            0.8, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y + 18,
                                                                            0.8, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            0.6, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                            CameraManager::EASE_OUT, 0, false);
                
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 90,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 40,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
                                                                           0.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 800,
                                                                           0.3, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 800,
                                                                           0.7, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z + 50,
                                                                           1.1, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                                           0.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE - 20,
                                                                             1.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE + 5,
                                                                             1.3, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D + 10,
                                                                             1.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D - 8,
                                                                             1.3, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D,
                                                                             0.8, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
            }
            
            
            // shun xi qian li
            if (styleID == 2)
            {
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarX,
//                                                                           0.1, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::EASE_OUT, 0);
                
  
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y + 3,
                                                                            1.6, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y - 5,
                                                                            0.7, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y + 5,
                                                                            0.7, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            1.0, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                            CameraManager::EASE_OUT, 0, false);
                
                
                
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y - 260,
                                                                           0.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y - 60,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 460,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
                                                                           2.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 800,
                                                                           0.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 800,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(INIT_CAMERA_Z - 800,
//                                                                           0.7, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z + 200,
                                                                           2.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(INIT_CAMERA_Z - 250,
//                                                                           1.8, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                                           0.5, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                
  
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE + 10,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE - 15,
                                                                             2.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                             2.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
                
                
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D + 10,
//                                                                             1.0, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::EASE_OUT, 0);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D - 8,
//                                                                             1.3, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::LINEAR, 0, false);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D,
//                                                                             0.8, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                                             CameraManager::EASE_OUT, 0, false);
            }

            
            
            if (styleID == 3)
            {
                // X & RY
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarX + 120,
                                                                           t, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, m_entityId.id());
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarX - dtx + 80,
                                                                           3.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, m_entityId.id(), false);
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y - 12,
                                                                            0.2, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            3.0, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                            CameraManager::EASE_OUT, 0, false);

                
                
                
                
              
//                // Y & R3D
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 200,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 240,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 120,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y - 50,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
                                                                           0.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                

                
                
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE - 20,
                                                                             0.2, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(INIT_CAMERA_ROTATE - 20,
//                                                                             3.7, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::LINEAR, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                             4.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);

                
                
                
                
                
                // Zoom
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 900,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(INIT_CAMERA_Z - 800,
//                                                                           0.7, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z + 350,
                                                                           2.1, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z,
                                                                           1.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 500,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 500,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                                           0.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);

                
                _BATTLE_STAGE.showBGColor(3, 133, 0, 255);
                
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D + 5,
//                                                                             3.0, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::EASE_OUT, 0);
//
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D + 12,
//                                                                             1.1, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::LINEAR, 0, false);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D - 10,
//                                                                             0.15, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::LINEAR, 0, false);
//
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D,
//                                                                             0.6, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                                             CameraManager::EASE_OUT, 0, false);

            }
            
            
            // xuan mu suo tian
            if (styleID == 4)
            {
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y - 9,
                                                                            0.9, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::EASE_OUT, 0);

                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y - 5,
                                                                            0.6, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y - 230,
                                                                           0.9, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y - 260,
                                                                           0.6, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 600,
                                                                           0.8, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 500,
                                                                           0.7, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                

                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE + 10,
                                                                             0.9, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE + 12,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);
//
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(INIT_CAMERA_ROTATE,
//                                                                             2.0, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                                             CameraManager::EASE_OUT, 0, false);
                
                
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D + 2,
//                                                                             0.8, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::EASE_OUT, 0);
////
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D - 2,
//                                                                             0.7, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::LINEAR, 0, false);
////
////                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D,
////                                                                             0.8, 0,
////                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
////                                                                             CameraManager::EASE_OUT, 0, false);
            }

            
            
            // xuan mu suo tian
            if (styleID == 5)
            {
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y - 25,
                                                                            0.0, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);

////                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(INIT_CAMERA_ROTATE_Y + 5,
////                                                                            0.7, 0,
////                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
////                                                                            CameraManager::LINEAR, 0, false);
////
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            2.4, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                            CameraManager::EASE_OUT, 0, false);

                
                

                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 700,
                                                                           0.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);

                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 700,
                                                                           0.8, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 100,
                                                                           0.3, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);

                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                                           1.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);

                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE - 12,
                                                                             0.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE - 10,
                                                                             0.8, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE + 8,
                                                                             0.2, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE + 10,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);

                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                             1.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
                
                


                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D,
                                                                             2.0, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
            }
            
            
            
            
            // qi bing
            if (styleID == 6)
            {
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            0.3, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y + 6,
                                                                            0.8, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y + 10,
                                                                            0.6, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            1.2, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                            CameraManager::EASE_OUT, 0, false);
                
                
                
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y + 70,
//                                                                           0.5, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::EASE_OUT, 0);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y + 20,
//                                                                           0.5, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::LINEAR, 0, false);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y + 20,
//                                                                           0.5, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::LINEAR, 0, false);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y - 120,
//                                                                           0.5, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                           CameraManager::LINEAR, 0, false);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y,
//                                                                           0.6, 0,
//                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 80,
                                                                           0.3, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 120,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 170,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);

                
                                                                           
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 800,
                                                                           0.3, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 800,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z + 340,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE - 20,
                                                                             0.5, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(INIT_CAMERA_ROTATE,
//                                                                             1.8, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                             2.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
                
                
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D + 10,
//                                                                             1.0, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::EASE_OUT, 0);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D - 8,
//                                                                             1.3, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                             CameraManager::LINEAR, 0, false);
//                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(INIT_CAMERA_ROTATE_2D,
//                                                                             0.8, 0,
//                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
//                                                                             CameraManager::EASE_OUT, 0, false);
            }
            
            
            
            
            
            
            // tian di wu yong
            if (styleID == 7)
            {
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            0.3, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y + 2,
                                                                            0.6, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                            CameraManager::LINEAR, 0, false);
                
//                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(INIT_CAMERA_ROTATE_Y - 2,
//                                                                            0.6, 0,
//                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
//                                                                            CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniRYEvent>(CameraManager::INIT_CAMERA_ROTATE_Y,
                                                                            1.0, 0,
                                                                            CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                            CameraManager::EASE_OUT, 0, false);
                
                
                
                
                //                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y + 70,
                //                                                                           0.5, 0,
                //                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                //                                                                           CameraManager::EASE_OUT, 0);
                //
                //                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y + 20,
                //                                                                           0.5, 0,
                //                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                //                                                                           CameraManager::LINEAR, 0, false);
                //
                //                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y + 20,
                //                                                                           0.5, 0,
                //                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                //                                                                           CameraManager::LINEAR, 0, false);
                //
                //                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y - 120,
                //                                                                           0.5, 0,
                //                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                //                                                                           CameraManager::LINEAR, 0, false);
                //
                //                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(INIT_CAMERA_Y,
                //                                                                           0.6, 0,
                //                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                //                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 80,
                                                                           0.3, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 320,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CONF_INIT_CAMERA_Y + 470,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 600,
                                                                           0.3, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::EASE_OUT, 0);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z - 600,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CONF_INIT_CAMERA_Z + 640,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                           CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                                           1.0, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                           CameraManager::EASE_OUT, 0, false);
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CONF_INIT_CAMERA_ROTATE - 20,
                                                                             0.5, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);
                
                //                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(INIT_CAMERA_ROTATE,
                //                                                                             1.8, 0,
                //                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                //                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                             2.1, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
                
                
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D + 4,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::EASE_OUT, 0);

                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D - 3,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                             CameraManager::LINEAR, 0, false);
                
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(CameraManager::INIT_CAMERA_ROTATE_2D,
                                                                             0.6, 0,
                                                                             CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                             CameraManager::EASE_OUT, 0, false);
            }


 
        }
    }

    remove();
}
