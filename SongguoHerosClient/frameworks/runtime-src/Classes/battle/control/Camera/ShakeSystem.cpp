//
//  ShakeSystem.cpp
//  SG
//
//  Created by fuchenhao on 3/16/15.
//
//

#include "ShakeSystem.h"

void ShakeSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
//    if(dt > MAX_INTERVAL)
//    {
//        dt = MAX_INTERVAL;
//    }
    
    auto cameraContainer = _CAMERA_MANAGER.getCameraContainer();

    BattleComponent::ShakeCom::Handle shake;
    for (entityx::Entity entity : es.entities_with_components(shake))
    {
        if(shake->delay > 0)
        {
            shake->delay -= dt;
        }
        else
        {
            if(shake->duration > 0)
            {
                shake->elapse += dt;
                
                if(shake->elapse >= shake->duration)
                {
                    cameraContainer->setPositionX(0);
                    cameraContainer->setPositionY(0);
                    cameraContainer->setPositionZ(0);
                    
                    entity.destroy();
                }
                else
                {
                    float shakeV = shake->amplitude;
                    
                    if (shake->damping)
                    {
                        shakeV = _CAMERA_MANAGER.calcTweenPos(shake->elapse, shake->amplitude, -shake->amplitude, shake->duration, CameraManager::EASE_OUT);
                    }
                    
                    int diceValueX = shake->direction;
                    int diceValueY = shake->direction;
                    int diceValueZ = shake->direction;

//                        CCLOG("========== ShakeSystem shakeTarget v: %f", shakeV);
//                        CCLOG("========== ShakeSystem shakeTarget d: %d", diceValueX);
//                        CCLOG("========== ShakeSystem mode: %d", shake->mode);
                    
                    switch (shake->mode)
                    {
                        case SHAKE_NONE:
//                                CCLOG("========== ShakeSystem NONE");
                            break;
                            
                        case SHAKE_X:
//                                CCLOG("========== ShakeSystem X");
                            cameraContainer->setPositionX(-diceValueX * shakeV);
                            break;
                            
                        case SHAKE_Y:
//                                CCLOG("========== ShakeSystem Y");
                            cameraContainer->setPositionY(diceValueY * shakeV);
                            break;
                            
                        case SHAKE_Z:
//                                CCLOG("========== ShakeSystem Z");
                            cameraContainer->setPositionZ(diceValueZ * shakeV);
                            break;
                            
                        case SHAKE_BOTH_XYZ:
//                                CCLOG("========== ShakeSystem XYZ");
                            cameraContainer->setPositionX(-diceValueX * shakeV);
                            cameraContainer->setPositionY(diceValueY * shakeV);
                            cameraContainer->setPositionZ(diceValueZ * shakeV);
                            break;
                            
                        default:
//                                CCLOG("========== ShakeSystem XY");
                            cameraContainer->setPositionX(-diceValueX * shakeV);
                            cameraContainer->setPositionY(diceValueY * shakeV);
                            break;
                    }
                    
//                        CCLOG("========== ShakeSystem inter: %d", shake->interval);
                    
                    shake->index++;
                    if (shake->index % shake->interval == 0)
                    {
                        shake->direction = -shake->direction;
                    }
                }
            }
        }
    }
}

/*
void ShakeSystem::setTargetOrgPos(BattleComponent::ShakeCom::Handle shake)
{
//    if (targets.)
    std::vector<cocos2d::Node*>::iterator result = find(targets.begin( ), targets.end( ), shake->shakeTarget);
    if (result == targets.end())
    {
        shake->orgX = shake->shakeTarget->getPositionX();
        shake->orgY = shake->shakeTarget->getPositionY();
        shake->orgZ = shake->shakeTarget->getPositionZ();
        targets.push_back(shake->shakeTarget);
        
        CCLOG("=============== camera x:: %f", shake->orgX);
        CCLOG("=============== camera y:: %f", shake->orgY);
        CCLOG("=============== camera z:: %f", shake->orgZ);
        
        //TO-DO remove temp code
        shake->orgX = 0;
        shake->orgY = 0;
        shake->orgZ = 0;
    }
    else
    {
//        CCLOG("==================Warning: shake overlap!!!");
    }
}

void ShakeSystem::removeTarget(cocos2d::Node* shakeTarget)
{
    for(auto it = targets.begin(); it != targets.end(); it++)
    {
        if(*it == shakeTarget)
        {
            targets.erase(it);
            break;
        }
    }
}*/

