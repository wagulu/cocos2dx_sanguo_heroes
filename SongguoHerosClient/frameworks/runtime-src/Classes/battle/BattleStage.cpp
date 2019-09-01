//
//  BattleStage
//
//  Created by fuchenhao on 3/1/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "BattleStage.h"
#include "DebugHelper.h"
#include "BattleComponent.h"
#include "TickerSystem.h"
#include "CameraSystem.h"
#include "SkillSystem.h"
#include "AttackSystem.h"
#include "GasSystem.h"
#include "SoldierEventSystem.h"
#include "HeroEventSystem.h"
#include "CameraEventSystem.h"
#include "BuffSystem.h"
#include "ActorModeSystem.h"
#include "AISystem.h"
#include "SoldierSystem.h"
#include "HeroSystem.h"
#include "ArrowSystem.h"
#include "EffectSystem.h"
#include "AnimationSystem.h"
#include "ShakeSystem.h"
#include "ComboSystem.h"
#include "AudioSystem.h"
#include "AnchorArrowSystem.h"
#include "BattleSystem.h"
#include "BattleWorld.h"

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    #include "TargetConditionals.h"
#endif

#if (TARGET_IPHONE_SIMULATOR)
// 在模拟器的情况下
    #define USE_FIX_DT  (false)
    #define FIX_DT  (1.0 / 10)
    #define MIN_INTERVAL        (1.0 / 60)
    #define MAX_INTERVAL        (1.0 / 10)
#elif (TARGET_OS_MAC)
// 在mac的情况下
    #define USE_FIX_DT  (false)
    #define FIX_DT  (1.0 / 60)
    #define MIN_INTERVAL        (1.0 / 60)
    #define MAX_INTERVAL        (1.0 / 30)
#else
// 在真机情况下
    #define USE_FIX_DT  (false)
    #define FIX_DT  (1.0 / 60)
    #define MIN_INTERVAL        (1.0 / 60)
    #define MAX_INTERVAL        (1.0 / 30)
#endif

#define FIELD_OF_VIEW       28

BattleStage::BattleStage(entityx::EntityX& entityX) : m_entityx(entityX)
{
    m_scheduler = cocos2d::Director::getInstance()->getScheduler();
    m_scheduler->scheduleUpdate(this, 0, false);
    
    m_entityx.systems.add<TickerSystem>();
    m_entityx.systems.add<BuffSystem>();
    m_entityx.systems.add<SoldierSystem>();
    m_entityx.systems.add<HeroSystem>();
    m_entityx.systems.add<AttackSystem>();
    m_entityx.systems.add<EffectSystem>();
    m_entityx.systems.add<ArrowSystem>();
    m_entityx.systems.add<CameraSystem>();
    m_entityx.systems.add<SkillSystem>();
    m_entityx.systems.add<GasSystem>();
    m_entityx.systems.add<SoldierEventSystem>();
    m_entityx.systems.add<HeroEventSystem>();
    m_entityx.systems.add<CameraEventSystem>();
    m_entityx.systems.add<ActorModeSystem>();
    m_entityx.systems.add<AISystem>();
    m_entityx.systems.add<AnimationSystem>();
    m_entityx.systems.add<ShakeSystem>();
    m_entityx.systems.add<ComboSystem>();
    m_entityx.systems.add<AnchorArrowSystem>();
    m_entityx.systems.add<AudioSystem>();
    m_entityx.systems.add<BattleSystem>();
    m_entityx.systems.configure();
   
    m_stage = cocos2d::Node::create();
    
    m_bg = cocos2d::Node::create();
    m_bg->setVisible(true);
    m_bg->setRotation3D(cocos2d::Vec3(-90, 0, 0));
    m_bg->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2, true);
    m_stage->addChild(m_bg);
    
    m_role = cocos2d::Node::create();
    m_role->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1, true);
    m_stage->addChild(m_role);
    
    m_battleSceneUI = BattleSceneUI::create();
    m_stage->addChild(m_battleSceneUI);
    
    m_battleDialogueUI = BattleDialogueUI::create();
    m_stage->addChild(m_battleDialogueUI);
    
    cocos2d::Rect visibleRect = cocos2d::Director::getInstance()->getOpenGLView()->getVisibleRect();
    
    m_bgCamera = SGCamera::createPerspective(FIELD_OF_VIEW, visibleRect.size.width/visibleRect.size.height, 1, 20000);
    m_bgCamera->setPositionZ(CameraManager::INIT_CAMERA_Z);
    m_bgCamera->setPositionY(CameraManager::INIT_CAMERA_Y);
    m_bgCamera->setPositionX(CameraManager::INIT_CAMERA_X);
    m_bgCamera->setRotation3D(cocos2d::Vec3(CameraManager::INIT_CAMERA_ROTATE, 0, 0));
    m_bgCamera->setCameraFlag(cocos2d::CameraFlag::USER2);
    
    m_roleCamera = (SGCamera*)(SGCamera::createPerspective(FIELD_OF_VIEW, visibleRect.size.width/visibleRect.size.height, 1, 20000));
    m_roleCamera->setPositionZ(CameraManager::INIT_CAMERA_Z);
    m_roleCamera->setPositionY(CameraManager::INIT_CAMERA_Y);
    m_roleCamera->setPositionX(CameraManager::INIT_CAMERA_X);
    m_roleCamera->setRotation3D(cocos2d::Vec3(CameraManager::INIT_CAMERA_ROTATE, 0, 0));
    m_roleCamera->setCameraFlag(cocos2d::CameraFlag::USER1);
    
    auto cameraBase = Node::create();
    cameraBase->addChild(m_bgCamera);
    cameraBase->addChild(m_roleCamera);
    m_stage->addChild(cameraBase);
    
    entityx::Entity camera = m_entityx.entities.create();
    camera.assign<BattleComponent::Camera>();
    camera.assign<BattleComponent::Position>(CameraManager::INIT_CAMERA_X, CameraManager::INIT_CAMERA_Y, CameraManager::INIT_CAMERA_Z);
    
    auto mapFrame = (cocos2d::Sprite*)m_battleSceneUI->getMap()->getChildByTag(MAP_FRAME);
    
    _CAMERA_MANAGER.initCamera(m_bgCamera, m_roleCamera, cameraBase, mapFrame, camera);
    
    m_battleTime = 0;
    m_battleType = BattleConfig::getInstance()->getBattleFieldConfig().battleType;
    m_pauseBattle = false;
    
    m_isMoving = false;
    m_isRebound = false;
    m_dragV = 0.0f;
    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(BattleStage::onTouchBegin, this);
    listener->onTouchesMoved = CC_CALLBACK_2(BattleStage::onTouchMove, this);
    listener->onTouchesEnded = CC_CALLBACK_2(BattleStage::onTouchEnd, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(BattleStage::onTouchCancel, this);
    m_stage->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_stage);
    
    m_showBackgroundRefCount = 0;
    m_pauseBattleRefCount = 0;
    m_backgroundColorTween.init(0, 255, 255);
}

BattleStage::~BattleStage()
{
    m_stage->removeFromParent();
    m_scheduler->unscheduleUpdate(this);
}

void BattleStage::update(float dt)
{
    int loopTimes = BattleWorld::getInstance()->getBattleSpeedTimes();
    for (int i = 0; i < loopTimes; ++i)
    {
        if (USE_FIX_DT)
        {
            dt = FIX_DT;
        }
        if (dt > MAX_INTERVAL) dt = MAX_INTERVAL;
        if (dt < MIN_INTERVAL) dt = MIN_INTERVAL;
        
        updataCameraMove(dt);
        
        //        // Slow Mode for testing
        //        m_timeRate = 6;
        dt /= m_timeRate;
        
        m_entityx.systems.update<BattleSystem>(dt);
        m_entityx.systems.update<TickerSystem>(dt);
        m_entityx.systems.update<CameraSystem>(dt);
        m_entityx.systems.update<ShakeSystem>(dt);
        m_entityx.systems.update<SkillSystem>(dt);
        m_entityx.systems.update<AnimationSystem>(dt);
        m_entityx.systems.update<EffectSystem>(dt);
        m_entityx.systems.update<ArrowSystem>(dt);
        m_entityx.systems.update<HeroSystem>(dt);
        m_entityx.systems.update<AttackSystem>(dt);
        m_entityx.systems.update<AudioSystem>(dt);
        
        if (!m_pauseBattle)
        {
            if (m_startFight)
            {
                m_entityx.systems.update<GasSystem>(dt);
                m_entityx.systems.update<BuffSystem>(dt);
                m_entityx.systems.update<ActorModeSystem>(dt);
                m_entityx.systems.update<AISystem>(dt);
            }
            
            m_entityx.systems.update<SoldierSystem>(dt);
            m_entityx.systems.update<AnchorArrowSystem>(dt);
            
            if (!m_backgroundColorTween.isFinish())
            {
                setBackgroundMaskColor(m_backgroundColorTween.out(dt));
            }
        }
        
        //do it at last
        _ENTITY_MANAGER.cleanWaitToDestroiedEntities();
        
#if (SHOW_HERO_TILE)
        DebugTools::showHeroTile();
#endif
    }
}

void BattleStage::pause()
{
    m_scheduler->pauseTarget(this);
}

void BattleStage::resume()
{
    m_scheduler->resumeTarget(this);
}

void BattleStage::updataCameraMove(float dt)
{
    if (_CAMERA_MANAGER.isCameraUserControlLocked())
    {
        m_dragV = 0;
        m_isRebound = false;
        m_isMoving = false;
        return;
    }
    
    
    if (m_isMoving)
    {
        m_dragDt += dt;
    }
    else
    {
        if (m_dragV != 0)
        {
            float curPos = _CAMERA_MANAGER.getCameraX();

            if(curPos <= CAMERA_MIN_X + 200  || curPos >= CAMERA_MAX_X - 200)
            {
//                CCLOG(">>>>>>>>>>> pos1: %f", curPos);
                if (!m_isRebound)
                {
                    const float vTimes = 4.0f;
                    if (curPos <= CAMERA_MIN_X + 200)
                    {
                        m_dragV = -fabs(m_dragV) / vTimes;
                        if (m_dragV < -1000) {
                            m_dragV = -1000;
                        }
//                        CCLOG(">>>>>>>>>>> to left");
                    }
                    else
                    {
                        m_dragV = fabs(m_dragV) / vTimes;
                        if (m_dragV > 1000) {
                            m_dragV = 1000;
                        }
//                        CCLOG(">>>>>>>>>>> to right");
                    }
                    
                    m_isRebound = true;
                }
            }
            
//            CCLOG(">>>>>>>>>>> v：%f", m_dragV);
            
//            _CAMERA_MANAGER.setCameraX(curPos - m_dragV * dt);
            _CAMERA_MANAGER.setCameraXSpring(curPos, - m_dragV * dt);
            
            if (m_dragV > 0)
            {
                if (m_dragV < 100) {
                    m_dragV -= 10;
                } else if (m_dragV < 200) {
                    m_dragV -= 20;
                } else {
                    m_dragV -= 40;
                }
                
//                CCLOG("============ v %f", m_dragV);
                
                if (m_dragV < 0)
                {
                    m_dragV = 0;
                    m_isRebound = false;
//                    _CAMERA_MANAGER.reboundCamera();
                }
            }
            else if (m_dragV < 0)
            {
                if (m_dragV > -100) {
                    m_dragV += 10;
                } else if (m_dragV > -200) {
                    m_dragV += 20;
                } else {
                    m_dragV += 40;
                }
                
//                CCLOG("============ v %f", m_dragV);
                
                if (m_dragV > 0)
                {
                    m_dragV = 0;
                    m_isRebound = false;
//                    _CAMERA_MANAGER.reboundCamera();
                }
            }
        }
    }
}

void BattleStage::changeTimeRate(float timeRate)
{
    if(!BattleWorld::getInstance()->isAutoQuickBattleMode())
    {
        m_timeRate = timeRate;
    }
}

void BattleStage::showBGColor(float duration)
{
    auto& children = m_bg->getChildren();
    for(const auto &child : children) {
        auto tint = cocos2d::Sequence::create(cocos2d::TintTo::create(duration, 255, 0, 0),
                                              cocos2d::TintTo::create(duration, 0, 255, 0),
                                              cocos2d::TintTo::create(duration, 0, 0, 255),
                                              nullptr);
        child->runAction(cocos2d::RepeatForever::create(tint));
    }
}

void BattleStage::showBGColor(float duration, GLubyte red, GLubyte green, GLubyte blue)
{
    auto& children = m_bg->getChildren();
    for(const auto &child : children) {
        auto tint = cocos2d::Sequence::create(cocos2d::TintTo::create(duration * 0.5, red, green, blue),
                                              cocos2d::TintTo::create(duration * 0.5, m_backgroundCurrentColor, m_backgroundCurrentColor, m_backgroundCurrentColor),
                                              nullptr);
        child->runAction(tint);
    }
}

void BattleStage::stopBGColor()
{
    auto& children = m_bg->getChildren();
    for(const auto &child : children) {
        child->stopAllActions();
    }
}

void BattleStage::showBackgroundMask(float duration, float color)
{
    if (duration <= 0)
    {
        setBackgroundMaskColor(color);
    }
    else
    {
        m_backgroundColorTween.init(duration, m_backgroundCurrentColor, color);
    }
    
    cout << "show back ground:" << m_showBackgroundRefCount << endl;
    m_showBackgroundRefCount++;
}

void BattleStage::hideBackgroundMask(float duration)
{
    m_showBackgroundRefCount--;
    cout << "hide back ground:" << m_showBackgroundRefCount << endl;
    
    if (m_showBackgroundRefCount == 0)
    {
        if (duration <= 0)
        {
            setBackgroundMaskColor(255);
        }
        else
        {
            m_backgroundColorTween.init(duration, m_backgroundCurrentColor, 255);
        }
        cout << "hide back ground..." << endl;
    }
}

void BattleStage::setBackgroundMaskColor(float value)
{
    auto& children = m_bg->getChildren();
    cocos2d::Color3B color(value, value, value);
    for(const auto &child : children)
    {
        child->setColor(color);
    }
    m_backgroundCurrentColor = value;
}

void BattleStage::pauseBattle()
{
    if(m_pauseBattleRefCount == 0)
    {
        m_pauseBattle = true;
        
        _ENTITY_EVENT.emit<BattleEvent::BattlePause>();
    }
    
    m_pauseBattleRefCount++;
}

void BattleStage::resumeBattle()
{
    m_pauseBattleRefCount--;
    
    if(m_pauseBattleRefCount == 0)
    {
        m_pauseBattle = false;
        
        _ENTITY_EVENT.emit<BattleEvent::BattleResume>();
    }
}

void BattleStage::onTouchBegin(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
    if (_ENTER_SHOW_CONTROL.isInEnterShow())
    {
        _ENTER_SHOW_CONTROL.clearEnterShow();
        return;
    }
    
    if (_CAMERA_MANAGER.isCameraUserControlLocked()) return;
    
    if(touches.size() >= 1)
    {
        m_dragDt = 0.0f;
        _CAMERA_MANAGER.clearCameraQueueX();
        m_isMoving = true;
    }
    
    if (touches.size() == 1)
    {
        m_startPos = touches[0]->getLocation();
        
        auto* mapBg = m_battleSceneUI->getMapBg();
        if(mapBg->getBoundingBox().containsPoint(mapBg->convertToNodeSpace(m_startPos)))
        {
            _CAMERA_MANAGER.setCameraX((mapBg->convertToNodeSpace(m_startPos).x - 30) * GRID_WIDTH / 4);
        }
    }
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_CLOSE_STRATEGY_PANEL);
}

void BattleStage::onTouchMove(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
    if (_CAMERA_MANAGER.isCameraUserControlLocked()) return;
    
    if (_ENTER_SHOW_CONTROL.isInEnterShow()) return;
    
    if(touches.size() >= 1 && m_isMoving)
    {
        auto* mapBg = m_battleSceneUI->getMapBg();
        if (mapBg->getBoundingBox().containsPoint(mapBg->convertToNodeSpace(touches[0]->getLocation())))
        {
            _CAMERA_MANAGER.setCameraX((mapBg->convertToNodeSpace(touches[0]->getLocation()).x - 30) * GRID_WIDTH / 4);
        }
        
        for (int i = 0; i < touches.size(); ++i)
        {
            auto touch = touches[i];

            _CAMERA_MANAGER.setCameraByVec2(touch->getPreviousLocation() - touch->getLocation());
        }
    }
}

void BattleStage::onTouchEnd(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
    if (_ENTER_SHOW_CONTROL.isInEnterShow()) return;
        
    if(touches.size() >= 1 && m_isMoving)
    {
        m_isRebound = false;
        
        float curPos = _CAMERA_MANAGER.getCameraX();
        if(curPos <= CAMERA_MIN_X + 200 || curPos >= CAMERA_MAX_X - 200)
        {
            _CAMERA_MANAGER.reboundCamera();
            m_dragV = 0;
        }
        else
        {
            if (fabs(touches[0]->getLocation().x - m_startPos.x) < 40) return;
            if (m_dragDt > 0.4f) return;
            
            if (m_dragDt > 0.0f)
                m_dragV = (touches[0]->getLocation().x - m_startPos.x) / m_dragDt;
            else
                m_dragV = 10000;
         }
    }

    m_isMoving = false;
}

void BattleStage::onTouchCancel(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
    onTouchEnd(touches, event);
}

void BattleStage::switchViewMode()
{
    if (fabs((CameraManager::INIT_CAMERA_Z) - 700) < 0.1)
    {
        CameraManager::INIT_CAMERA_ROTATE = -24.0f;
        CameraManager::INIT_CAMERA_Y = 410.0f;
        CameraManager::INIT_CAMERA_Z = 100.0f;
        
        CCLOG("======= camera zoom in view");
    }
    else
    {
        CameraManager::INIT_CAMERA_ROTATE = -14.0f;
        CameraManager::INIT_CAMERA_Y = 370.0f;
        CameraManager::INIT_CAMERA_Z = 700.0f;
        
        CCLOG("======= camera default view");
    }
    
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(CameraManager::INIT_CAMERA_Y,
                                                               0.2, 0,
                                                               CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                               CameraManager::EASE_OUT, -1);
    
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(CameraManager::INIT_CAMERA_Z,
                                                               0.2, 0,
                                                               CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                               CameraManager::EASE_OUT, -1);
    
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(CameraManager::INIT_CAMERA_ROTATE,
                                                                 0.2, 0,
                                                                 CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                 CameraManager::EASE_OUT, -1);
}
