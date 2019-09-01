//
//  CameraManager.h
//  SG
//
//  Created by fuchenhao on 3/3/15.
//
//

#ifndef __CAMERA_MANAGER_H__
#define __CAMERA_MANAGER_H__

#include "BattleConfig.h"
#include "BattleEvent.h"
#include "BattleComponent.h"

#include "SGCamera.h"
#include "CameraTwins.h"

#include "../../../entityx/entityx.h"

class CameraManager
{
    
public:
    static float INIT_CAMERA_ROTATE;
    static float INIT_CAMERA_ROTATE_2D;
    static float INIT_CAMERA_ROTATE_Y;
    
    static float INIT_CAMERA_X;
    static float INIT_CAMERA_Y;
    static float INIT_CAMERA_Z;

    enum CameraMode
    {
        CAMERA_FREE,                //0
        CAMERA_LOCKED,
        CAMERA_END,
        
        CAMERA_MOVE,
        
        CAMERA_SINUS,
        CAMERA_FOLLOW,
        CAMERA_FOLLOW_CAN_BREAK,
        
        CAMERA_ZOOMING,
        CAMERA_ZOOMED
    };

    enum tweenMode
    {
        LINEAR,
        EASE_IN,
        EASE_OUT,
        EASE_IN_OUT
    };
    
    CameraManager();
    virtual ~CameraManager();
    
    void initCamera(SGCamera* bgCamera, SGCamera* roleCamera,
                    cocos2d::Node* cameraContainer,
                    cocos2d::Sprite* frame, entityx::Entity& camera);

    void setCameraModeX(int mode);
    void setCameraModeY(int mode);
    void setCameraModeZ(int mode);
    void setCameraModeR3d(int mode);
    void setCameraModeR2d(int mode);
    void setCameraModeRY(int mode);
    
    void backToLastMode();
    void setCameraModeFollow(bool following, bool canBreak = false);
    
    float getCameraX();
    float getCameraZ();
    
    bool isCameraYDefault();
    bool isCameraZDefault();
    bool isCameraR3dDefault();
    bool isCameraR2dDefault();
    bool isCameraRYDefault();
    
    void setCameraXSpring(float o, float d);
    void reboundCamera();
    
    bool isCameraUserControlLocked();
    bool isCameraSinus();
    bool isCameraFollow();
    int getCameraFollowTargetId();
    
    void setCameraX(float x);
    void setCameraXFree(float x);
    void setCameraZ(float x);
    void setCameraZFree(float x);
//    static void setCameraY(float x);
    void setCameraYFree(float x);
    
    float getCameraRotation3D();
    void setCameraRotation3D(float rotate);
    float getCameraRotation2D();
    void setCameraRotation2D(float rotate);
    float getCameraRotationY();
    void setCameraRotationY(float rotate);
    
    void setCameraByVec2(const cocos2d::Vec2& dt);
    void setCameraByAction(const BattleConfig::CameraAction& cameraAction, BattleComponent::Position::Handle& position);
    void setCameraByAction(float toX, float toY, float toZ, float toRotate, BattleComponent::Position::Handle& position);
    
    void setCameraByActionX(float toX, BattleComponent::Position::Handle& position);
    void setCameraByActionY(float toY, BattleComponent::Position::Handle& position);
    void setCameraByActionZ(float toZ, BattleComponent::Position::Handle& position);
    
    void setCameraFollowTarget(const entityx::Entity& entity);
    void moveCameraFollowX(entityx::Entity& entity, BattleComponent::Position::Handle& positionCam);
    
    bool inCameraView(float pointX);
    
    void initSmartFollowSolider(bool frontEnd = true);
    void smartFollowSolider(bool frontEnd = true, bool normalUpdate = true);
    
    void updateCameraPos(BattleComponent::Position::Handle& position);
    
    bool isInTwoCameraMode();
    
//    void setCameraShake(float duration = 0.0f, float amplitude = 0.0f, float delay = 0.0f);
    
    void clearCameraQueue();
    void clearCameraQueueX();
    void clearCameraQueueY();
    void clearCameraQueueZ();
    void clearCameraQueueR3d();
    void clearCameraQueueR2d();
    void clearCameraQueueRY();
    
    void addCameraQueueToX(float toX, float time, float delay, int nextCamState, float tweenMode);
    void addCameraQueueToY(float toY, float time, float delay, int nextCamState, float tweenMode, float backDelay = 1.0f);
    void addCameraQueueToZ(float toZ, float time, float delay, int nextCamState, float tweenMode, float backDelay = 1.0f);
    void addCameraQueueToR3d(float r3d, float time, float delay, int nextCamState, float tweenMode, float backDelay = 1.0f);
    void addCameraQueueToR2d(float r2d, float time, float delay, int nextCamState, float tweenMode, float backDelay = 10.0f);
    void addCameraQueueToRY(float r2d, float time, float delay, int nextCamState, float tweenMode, float backDelay = 10.0f);
    
    void addCameraQueueTo(float toX, float toY, float toZ, float rotate, float time, float delay, int nextCamState, float tweenMode);
    
    float calcTweenPos(float t, float b, float c, float d, int mode);
    void setCameraUserControlLocked(bool locked);
    
    uint64_t getCameraCauseID();
    void setCameraCauseID(uint64_t id);
    
    CameraTwins& getMainCamera();
    cocos2d::Node* getCameraContainer();

private:
    void initCamera(CameraTwins* mainCamera, cocos2d::Sprite* frame, entityx::Entity& camera);
    void initCamera(CameraTwins* mainCamera, CameraTwins* sedCamera,
                    cocos2d::Sprite* frame, entityx::Entity& camera);
    void initCamera(SGCamera* bgCamera, SGCamera* roleCamera,
                    cocos2d::Sprite* frame, entityx::Entity& camera);

    
    void addCameraQueue(float toX, float toY, float toZ, float toR, float time, float delay, int nextCamState, float tweenMode);
    void addCameraQueue(float fromX, float toX, float fromY, float toY, float fromZ, float toZ, float fromR, float toR, float time, float delay, float tweenMode);

    BattleComponent::Camera::Handle s_cameraCom;
    
    CameraTwins* s_mainCamera;
    
    // miMap
    cocos2d::Sprite* s_frame;
    
    cocos2d::Node* m_cameraContainer;
    
    void configEvent(entityx::EventManager& events);
    
    bool s_inTwoCamerasMode;
    
    bool m_inSoliderFollowMode = false;
    bool m_userControlLocked = false;
};

#endif
