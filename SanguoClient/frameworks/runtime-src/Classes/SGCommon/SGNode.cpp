//
//  SGCamera.cpp
//  SGCamera
//
//  Created by fuchenhao on 1/4/15.
//
//
/*
#include "SGNode.h"

#include "CameraManager.h"
#include "BattleWorld.h"

SGNode* SGNode::create()
{
    SGNode * ret = new (std::nothrow) SGNode();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SGNode::init()
{
    glEnable(GL_SCISSOR_TEST);
    resetClippingPos();
    removeDone = true;
    return true;
}

void SGNode::resetClippingPos()
{
    clippingPos = 0;
}

void SGNode::roleClipping(GLint x, GLint y, GLsizei width, GLsizei height,
                          cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    glEnable(GL_SCISSOR_TEST);
    float tX = BattleConfig::getInstance()->widthRate;
    float tY = BattleConfig::getInstance()->heightRate;
//    CCLOG("========== roleClipping: %f / %f", tX, tY);
    glScissor(x * tX, y * tY, width * tX, height * tY);
    Node::visit(renderer, parentTransform, parentFlags);
    glDisable(GL_SCISSOR_TEST);
}

void SGNode::cocosClipping(float x, float y, float width, float height)
{
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    
    glview->setScissorInPoints(x, y, width, height);
    glEnable(GL_SCISSOR_TEST);
}

void SGNode::visitTwoCameras(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
//    CCLOG("======= SGNode visitTwoCameras");
    auto camera = cocos2d::Camera::getVisitingCamera();
    
    auto camera1 = getRoleCamera(_CAMERA_MANAGER.getMainCamera());
//    auto camera2 = getRoleCamera(CameraManager::getSecondCamera());
    auto camera3 = getBGCamera(_CAMERA_MANAGER.getMainCamera());
    auto camera4 = getBGCamera(_CAMERA_MANAGER.getSecondCamera());
    
    cocos2d::Rect visibleRect = cocos2d::Director::getInstance()->getOpenGLView()->getVisibleRect();
    float barWidth = 2;
    float leftWidth = (visibleRect.size.width - barWidth)/2;
    
    if (camera == camera1) {
        roleClipping(0, 0, leftWidth, visibleRect.size.height, renderer, parentTransform, parentFlags);
    }
    else {
        roleClipping(leftWidth + barWidth, 0, leftWidth, visibleRect.size.height, renderer, parentTransform, parentFlags);
    }
    
    if(camera == camera3) {
        cocosClipping(0, 0, leftWidth, visibleRect.size.height);
    }
    else if(camera == camera4) {
        cocosClipping(leftWidth + barWidth, 0, leftWidth, visibleRect.size.height);
    }
}

void SGNode::removeCamera2(float dt)
{
    _BATTLE_STAGE.removeCamera2();
    removeDone = true;
    
    unschedule(schedule_selector(SGNode::removeCamera2));
}

SGCamera* SGNode::getRoleCamera(CameraTwins &inTwin)
{
    return inTwin.m_roleCamera;
}

SGCamera* SGNode::getBGCamera(CameraTwins &inTwin)
{
    return inTwin.m_bgCamera;
}

void SGNode::visitTwoCamerasWithAni(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
//    CCLOG("======= SGNode visitTwoCamerasWithAni");
    auto camera = cocos2d::Camera::getVisitingCamera();

    auto camera1 = getRoleCamera(_CAMERA_MANAGER.getMainCamera());
    auto camera2 = getRoleCamera(_CAMERA_MANAGER.getSecondCamera());
    
    auto camera3 = getBGCamera(_CAMERA_MANAGER.getMainCamera());
    auto camera4 = getBGCamera(_CAMERA_MANAGER.getSecondCamera());

    float moveSpeed = 8;
    
    cocos2d::Rect visibleRect = cocos2d::Director::getInstance()->getOpenGLView()->getVisibleRect();
    float barWidth = 10;
    float leftWidth = (visibleRect.size.width - barWidth)/2;
    
    if (camera1->getCameraStatus() == SGCamera::CAMERA_LEFT &&
        camera2->getCameraStatus() == SGCamera::CAMERA_NONE) {
        
        clippingPos += moveSpeed;
        float rightWidth = clippingPos;

        if (camera == camera1) {
            roleClipping(0, 0, visibleRect.size.width - barWidth - rightWidth, visibleRect.size.height,
                         renderer, parentTransform, parentFlags);
        }
        else {
            roleClipping(visibleRect.size.width + barWidth - rightWidth, 0, rightWidth, visibleRect.size.height,
                         renderer, parentTransform, parentFlags);
        }
        
        if(camera == camera3) {
            cocosClipping(0, 0, visibleRect.size.width - barWidth - rightWidth, visibleRect.size.height);
        }
        else if(camera == camera4) {
            cocosClipping(visibleRect.size.width + barWidth - rightWidth, 0, rightWidth, visibleRect.size.height);
        }
        
        if (clippingPos >= leftWidth) {
            clippingPos = (visibleRect.size.width - barWidth)/2;
            camera1->setCameraStatus(SGCamera::CAMERA_LEFT);
            camera2->setCameraStatus(SGCamera::CAMERA_RIGHT);
            
            // TODO chance
//            CameraControl::getInstance()->setTwoCameraLock(false);
        }
    }
    else if(camera1->getCameraStatus() == SGCamera::CAMERA_ALL &&
            camera2->getCameraStatus() == SGCamera::CAMERA_RIGHT) {
        removeDone = false;

        clippingPos -= moveSpeed;
        float rightWidth = clippingPos;
        
        if (camera == camera1) {
            roleClipping(0, 0, visibleRect.size.width - barWidth - rightWidth, visibleRect.size.height,
                         renderer, parentTransform, parentFlags);
        }
        else {
            roleClipping(visibleRect.size.width + barWidth - rightWidth, 0, rightWidth, visibleRect.size.height,
                         renderer, parentTransform, parentFlags);
        }
        
        if(camera == camera3) {
            cocosClipping(0, 0, visibleRect.size.width - barWidth - rightWidth + 34, visibleRect.size.height);
        }
        else if(camera == camera4) {
            cocosClipping(visibleRect.size.width + barWidth - rightWidth, 0, rightWidth, visibleRect.size.height);
        }
        
        if (clippingPos <= 0) {
            clippingPos = 0;
            camera1->setCameraStatus(SGCamera::CAMERA_ALL);
            camera2->setCameraStatus(SGCamera::CAMERA_NONE);
            
            scheduleOnce(schedule_selector(SGNode::removeCamera2), 0);
        }
    }
    
    else {
        if (removeDone) {
            visitTwoCameras(renderer, parentTransform, parentFlags);
        } else {
            if (camera == camera1) {
                roleClipping(0, 0, visibleRect.size.width, visibleRect.size.height,
                             renderer, parentTransform, parentFlags);
            }
            else {
                roleClipping(visibleRect.size.width + barWidth, 0, 0, 0,
                             renderer, parentTransform, parentFlags);
            }
            
            if(camera == camera3) {
                cocosClipping(0, 0, visibleRect.size.width, visibleRect.size.height);
            }
            else if(camera == camera4) {
                cocosClipping(visibleRect.size.width + barWidth, 0, 0, 0);
            }
        }
    }
}

void SGNode::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
//    CCLOG("======= SGNode visit");
    
//    bool camStatus = CameraControl::getInstance()->isInTwoCameraMode();
    bool camStatus = _CAMERA_MANAGER.isInTwoCameraMode();
    
    if (camStatus) {
//        visitTwoCameras(renderer, parentTransform, parentFlags);
        visitTwoCamerasWithAni(renderer, parentTransform, parentFlags);
    } else {
        glDisable(GL_SCISSOR_TEST);
        Node::visit(renderer, parentTransform, parentFlags);
    }
}*/
