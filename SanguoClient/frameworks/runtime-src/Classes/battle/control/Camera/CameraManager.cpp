//
//  CameraManager.cpp
//  SG
//
//  Created by fuchenhao on 3/3/15.
//
//

#include "CameraManager.h"
#include "BattleWorld.h"


float CameraManager::INIT_CAMERA_ROTATE = CONF_INIT_CAMERA_ROTATE;
float CameraManager::INIT_CAMERA_ROTATE_2D = 0.0f;
float CameraManager::INIT_CAMERA_ROTATE_Y = 0.0f;

float CameraManager::INIT_CAMERA_X = 700.0f;
float CameraManager::INIT_CAMERA_Y = CONF_INIT_CAMERA_Y;
float CameraManager::INIT_CAMERA_Z = CONF_INIT_CAMERA_Z;

CameraManager::CameraManager()
{
//    BattleComponent::Camera::Handle CameraManager::s_cameraCom;
    s_mainCamera   = nullptr;
    
    s_frame        = nullptr;
    
    s_inTwoCamerasMode     = false;
    m_inSoliderFollowMode  = false;
}

CameraManager::~CameraManager()
{
    CC_SAFE_DELETE(s_mainCamera);
}



void CameraManager::initCamera(SGCamera* bgCamera, SGCamera* roleCamera,
                               cocos2d::Sprite* frame, entityx::Entity& camera)
{
    s_mainCamera = new CameraTwins(roleCamera, bgCamera);
    
    initCamera(nullptr, frame, camera);
}

void CameraManager::initCamera(CameraTwins* mainCamera,
                               cocos2d::Sprite* frame, entityx::Entity& camera)
{
    s_frame = frame;
    
    s_cameraCom = camera.component<BattleComponent::Camera>();
    s_cameraCom->cameraModeX = CAMERA_LOCKED;
    s_cameraCom->cameraLastMode = CAMERA_LOCKED;
    
    s_cameraCom->cameraModeY = CAMERA_LOCKED;
    s_cameraCom->cameraModeZ = CAMERA_LOCKED;
    s_cameraCom->cameraModeR3d = CAMERA_LOCKED;
    s_cameraCom->cameraModeR2d = CAMERA_LOCKED;
}

void CameraManager::initCamera(SGCamera* bgCamera, SGCamera* roleCamera,
                               cocos2d::Node* cameraContainer,
                               cocos2d::Sprite* frame, entityx::Entity& camera)
{
    m_cameraContainer = cameraContainer;
    initCamera(bgCamera, roleCamera, frame, camera);
}



CameraTwins& CameraManager::getMainCamera()
{
    return *s_mainCamera;
}


void CameraManager::setCameraModeX(int mode)
{
//    if(s_cameraCom->cameraModeX == CameraManager::CAMERA_END) return;
    if(s_cameraCom->cameraModeX != mode)
    {
        s_cameraCom->cameraLastMode = s_cameraCom->cameraModeX;
        s_cameraCom->cameraModeX = mode;
    }
}

void CameraManager::setCameraModeY(int mode)
{
    if(s_cameraCom->cameraModeY != mode)
    {
        s_cameraCom->cameraModeY = mode;
    }
}

void CameraManager::setCameraModeZ(int mode)
{
    if(s_cameraCom->cameraModeZ != mode)
    {
        s_cameraCom->cameraModeZ = mode;
    }
}

void CameraManager::setCameraModeR3d(int mode)
{
    if(s_cameraCom->cameraModeR3d != mode)
    {
        s_cameraCom->cameraModeR3d = mode;
    }
}

void CameraManager::setCameraModeR2d(int mode)
{
    if(s_cameraCom->cameraModeR2d != mode)
    {
        s_cameraCom->cameraModeR2d = mode;
    }
}

void CameraManager::setCameraModeRY(int mode)
{
    if(s_cameraCom->cameraModeRY != mode)
    {
        s_cameraCom->cameraModeRY = mode;
    }
}






void CameraManager::backToLastMode()
{
    s_cameraCom->cameraModeX = s_cameraCom->cameraLastMode;
    s_cameraCom->cameraLastMode = CameraManager::CAMERA_FREE;
}

void CameraManager::setCameraModeFollow(bool following, bool canBreak)
{
    if (following)
    {
        if (canBreak)
        {
            setCameraModeX(CameraManager::CAMERA_FOLLOW_CAN_BREAK);
        }
        else
        {
            setCameraModeX(CameraManager::CAMERA_FOLLOW);
        }
    }
    else
    {
        if(s_cameraCom->cameraModeX == CameraManager::CAMERA_FOLLOW ||
           s_cameraCom->cameraModeX == CameraManager::CAMERA_FOLLOW_CAN_BREAK)
        {
            backToLastMode();
        }
    }
}



//============ pos ==============


float CameraManager::getCameraX()
{
    return s_mainCamera->getCameraPosX();
}

float CameraManager::getCameraZ()
{
    return s_mainCamera->getCameraPosZ();
}

void CameraManager::setCameraXSpring(float o, float d)
{
    if(s_cameraCom->cameraModeX == CAMERA_LOCKED) return;

    float x = o + d;
    
    if(x < CAMERA_MIN_X)
    {
        x = o + d / 3;
    }
    if(x > CAMERA_MAX_X)
    {
        x = o + d / 3;
    }
    
    setCameraXFree(x);
}

void CameraManager::setCameraX(float x)
{
    if(x < CAMERA_MIN_X) x = CAMERA_MIN_X;
    if(x > CAMERA_MAX_X) x = CAMERA_MAX_X;

    setCameraXFree(x);
}

void CameraManager::setCameraZ(float z)
{
    setCameraZFree(z);
}

void CameraManager::setCameraXFree(float x)
{
    s_mainCamera->setCameraPosXTo(x);
    
    int tileX = x / GRID_WIDTH;
    s_frame->setPositionX(tileX * 4);
}

void CameraManager::setCameraZFree(float z)
{
    s_mainCamera->setCameraPosZTo(z);
}

void CameraManager::setCameraYFree(float y)
{
    s_mainCamera->setCameraPosYTo(y);
}


float CameraManager::getCameraRotation3D()
{
    return s_mainCamera->getCameraRotation3D();
}

void CameraManager::setCameraRotation3D(float rotate)
{
    s_mainCamera->setCameraRotationTo3D(rotate);
}

float CameraManager::getCameraRotation2D()
{
    return s_mainCamera->getCameraRotation2D();
}

void CameraManager::setCameraRotation2D(float rotate)
{
    s_mainCamera->setCameraRotationTo2D(rotate);
}

float CameraManager::getCameraRotationY()
{
    return s_mainCamera->getCameraRotationY();
}

void CameraManager::setCameraRotationY(float rotate)
{
    s_mainCamera->setCameraRotationToY(rotate);
}

const float EPSINON = 0.0001f;
#define IS_EQUAL(val1, val2)  (fabs((val1) - (val2)) <= EPSINON)

bool CameraManager::isCameraYDefault()
{
    return IS_EQUAL(s_mainCamera->getCameraPosY(), INIT_CAMERA_Y);
}

bool CameraManager::isCameraZDefault()
{
    return IS_EQUAL(s_mainCamera->getCameraPosZ(), INIT_CAMERA_Z);
}

bool CameraManager::isCameraR3dDefault()
{
    return IS_EQUAL(s_mainCamera->getCameraRotation3D(), INIT_CAMERA_ROTATE);
}

bool CameraManager::isCameraR2dDefault()
{
    return IS_EQUAL(s_mainCamera->getCameraRotation2D(), INIT_CAMERA_ROTATE_2D);
}

bool CameraManager::isCameraRYDefault()
{
    return IS_EQUAL(s_mainCamera->getCameraRotationY(), INIT_CAMERA_ROTATE_Y);
}


void CameraManager::moveCameraFollowX(entityx::Entity& entity, BattleComponent::Position::Handle& positionCam)
{
    if(s_cameraCom->cameraModeX == CAMERA_FOLLOW ||
       s_cameraCom->cameraModeX == CAMERA_FOLLOW_CAN_BREAK)
    {
        auto positionTar = entity.component<BattleComponent::Position>();
        float followDistance = FOLLOW_DISTANCE;
        if(entity.has_component<BattleComponent::Identify>())
        {
            auto identify = entity.component<BattleComponent::Identify>();
            followDistance = (identify->side == BattleConfig::SIDE_LEFT) ? FOLLOW_DISTANCE : -FOLLOW_DISTANCE;
        }
        else if(entity.has_component<BattleComponent::SkillArrow>())
        {
            auto& arrowData = entity.component<BattleComponent::SkillArrow>()->arrowData;
            followDistance = (arrowData.side == BattleConfig::SIDE_LEFT) ? FOLLOW_DISTANCE : -FOLLOW_DISTANCE;
        }
        positionCam->x = positionTar->x + followDistance;
        setCameraX(positionTar->x + followDistance);
    }
}

void CameraManager::setCameraFollowTarget(const entityx::Entity& entity)
{
    s_cameraCom->followData.followTarget = entity;
}

int CameraManager::getCameraFollowTargetId()
{
    int result = -1;
    if (s_cameraCom->followData.followTarget.valid())
    {
        auto identify = (s_cameraCom->followData.followTarget).component<BattleComponent::Identify>();
        
//        if (identify == NULL) return result;
        if (!identify.valid()) return result;
        
        if (identify->type == BattleConfig::ENTITY_HERO)
        {
            if (identify->side ==BattleConfig::SIDE_LEFT)
            {
                result = LEFT_HERO;
            }
            else
            {
                result = RIGHT_HERO;
            }
        }
    }
    
    return result;
}

void CameraManager::reboundCamera()
{
    float x = s_mainCamera->getCameraPosX();

    if(x <= CAMERA_MIN_X + 200)
    {
        addCameraQueueToX(CAMERA_MIN_X + 350, 0.17, 0, CameraManager::CAMERA_FREE, CameraManager::EASE_OUT);
        setCameraModeX(CameraManager::CAMERA_ZOOMING);
    }
    if(x >= CAMERA_MAX_X - 200)
    {
        addCameraQueueToX(CAMERA_MAX_X - 350, 0.17, 0, CameraManager::CAMERA_FREE, CameraManager::EASE_OUT);
        setCameraModeX(CameraManager::CAMERA_ZOOMING);
    }
}


bool CameraManager::inCameraView(float pointX)
{
    bool inView = false;
    
    float curPos = this->getCameraX();
    
    if (abs(curPos - pointX) < 530)
    {
        inView = true;
    }
    
    return inView;
}


void CameraManager::initSmartFollowSolider(bool frontEnd)
{
    if (_CAMERA_MANAGER.isCameraUserControlLocked())
    {
        _ENTITY_EVENT.emit<BattleEvent::SetCameraModeForLockEvent>(true);
        return;
    }
    
    float s_maxPosX = -1;
    std::vector<entityx::Entity> &soldiers = _BATTLE_ENTITY.getLeftSoldiers();
    for(int i = 0; i < soldiers.size(); i++)
    {
        auto position = (soldiers[i]).component<BattleComponent::Position>();
        
        BattleComponent::Soldier::Handle soldierHandle = (soldiers[i]).component<BattleComponent::Soldier>();
        if (soldierHandle->config.soldierType == BattleConfig::SoldierType::BLOCK) continue;

        if (!frontEnd)
        {
            if (floor(position->x) < floor(s_maxPosX) || s_maxPosX == -1)
            {
                s_maxPosX = position->x;
            }
        }
        else
        {
            if (floor(position->x) > floor(s_maxPosX) || s_maxPosX == -1)
            {
                s_maxPosX = position->x;
            }
        }
    }

    if (s_maxPosX != -1)
    {
        float curPos = this->getCameraX();
        float tarPos = s_maxPosX - 50;

        bool inView = this->inCameraView(tarPos);
        
        if (!inView)
        {
            _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarPos,
                                                                       0.2, 0,
                                                                       CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_SINUS,
                                                                       CameraManager::EASE_IN, -3, true);
        }
        else
        {
            if (tarPos < curPos)
            {
                this->setCameraModeX(CameraManager::CAMERA_SINUS);
            }
            else
            {
                _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarPos,
                                                                           0.2, 0,
                                                                           CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_SINUS,
                                                                           CameraManager::EASE_IN, -3, true);
            }
        }
    }
}


void CameraManager::smartFollowSolider(bool frontEnd, bool normalUpdate)
{
    float s_maxPosX = -1;
    std::vector<entityx::Entity> &soldiers = _BATTLE_ENTITY.getLeftSoldiers();
    for(int i = 0; i < soldiers.size(); i++)
    {
        auto position = (soldiers[i]).component<BattleComponent::Position>();
        
        BattleComponent::Soldier::Handle soldierHandle = (soldiers[i]).component<BattleComponent::Soldier>();
        if (soldierHandle->config.soldierType == BattleConfig::SoldierType::BLOCK) continue;
        
        if (!frontEnd)
        {
            if (floor(position->x) < floor(s_maxPosX) || s_maxPosX == -1)
            {
                s_maxPosX = position->x;
            }
        }
        else
        {
            if (floor(position->x) > floor(s_maxPosX) || s_maxPosX == -1)
            {
                s_maxPosX = position->x;
            }
        }
    }
    
    if (s_maxPosX != -1)
    {
        float curPos = this->getCameraX();
        float tarPos = s_maxPosX - 50;
        
        bool inView = this->inCameraView(tarPos);

        if (tarPos > curPos)
        {
            this->setCameraX(tarPos);
            m_inSoliderFollowMode = true;
        }
        
        if (m_inSoliderFollowMode && tarPos < curPos - 200)
        {
            _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarPos,
                                                                       1, 0,
                                                                       CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_SINUS,
                                                                       CameraManager::EASE_OUT, -3, true);
        }
    }
    else
    {
        auto* pEntity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
        if (pEntity)
        {
            _ENTITY_EVENT.emit<BattleEvent::CameraFollowEntity>(*pEntity);
        }
    }
}


bool CameraManager::isCameraUserControlLocked()
{
    return m_userControlLocked || (s_cameraCom->cameraModeX == CAMERA_END);
}

void CameraManager::setCameraUserControlLocked(bool locked)
{
    m_userControlLocked = locked;
}

bool CameraManager::isCameraSinus()
{
    return (s_cameraCom->cameraModeX == CAMERA_SINUS);
}

bool CameraManager::isCameraFollow()
{
    return (s_cameraCom->cameraModeX == CAMERA_FOLLOW ||
            s_cameraCom->cameraModeX == CAMERA_FOLLOW_CAN_BREAK);
}


void CameraManager::setCameraByVec2(const cocos2d::Vec2& dt)
{
    if (_CAMERA_MANAGER.isCameraUserControlLocked()) return;
    
    if(s_cameraCom->cameraModeX == CAMERA_FOLLOW_CAN_BREAK)
    {
        setCameraModeX(CAMERA_FREE);
    }
    if(s_cameraCom->cameraModeX == CAMERA_FREE)
    {
        setCameraCauseID(-1);
        float prevX = s_mainCamera->getCameraPosX();
        setCameraXSpring(prevX, dt.x);
    }
}


void CameraManager::updateCameraPos(BattleComponent::Position::Handle& position)
{
    position->x = s_mainCamera->getCameraPosX();
    position->y = s_mainCamera->getCameraPosY();
    position->z = s_mainCamera->getCameraPosZ();
}

void CameraManager::setCameraByActionX(float toX, BattleComponent::Position::Handle& position)
{
    position->x = toX;
    
    setCameraXFree(toX);
}

void CameraManager::setCameraByActionY(float toY, BattleComponent::Position::Handle& position)
{
    position->y = toY;
    if (s_cameraCom->cameraModeY == CameraManager::CAMERA_ZOOMING)
    {
        CameraManager::setCameraYFree(toY);
    }
}

void CameraManager::setCameraByActionZ(float toZ, BattleComponent::Position::Handle& position)
{
    position->z = toZ;

    if (s_cameraCom->cameraModeZ == CameraManager::CAMERA_ZOOMING)
    {
        CameraManager::setCameraZFree(toZ);
    }
    else
    {
        CameraManager::setCameraZ(toZ);
    }
}




float CameraManager::calcTweenPos(float t, float b, float c, float d, int mode)
{
    switch (mode) {
        case EASE_IN:
            t /= d;
            return c * (t) * t * t * t + b;
            
        case EASE_OUT:
            t /= d;
            return -c * (t) * (t - 2) + b;
            
        case EASE_IN_OUT:
            t /= d;
            if ((t / 2) < 1) return c / 2 * t * t * t * t + b;

            t -= 2;
            return -c / 2 * ((t) * t * t * t - 2) + b;
        
        default:
            return c * t / d + b;
    }
}

cocos2d::Node* CameraManager::getCameraContainer()
{
    return m_cameraContainer;
}



//////  queue //////////////
void CameraManager::clearCameraQueue()
{
    clearCameraQueueX();
    clearCameraQueueY();
    clearCameraQueueZ();
    clearCameraQueueR3d();
    clearCameraQueueR2d();
    clearCameraQueueRY();
}

void CameraManager::clearCameraQueueX()
{
    //    setCameraFollowFlag(false);
    
    if(s_cameraCom->cameraModeX == CAMERA_LOCKED) return;
    setCameraModeX(CameraManager::CAMERA_FREE);
    s_cameraCom->cameraXQueue.clear();
}

void CameraManager::clearCameraQueueY()
{
    //    setCameraFollowFlag(false);
    
    if(s_cameraCom->cameraModeY == CAMERA_LOCKED) return;
    setCameraModeY(CameraManager::CAMERA_FREE);
    s_cameraCom->cameraYQueue.clear();
}

void CameraManager::clearCameraQueueZ()
{
    //    setCameraFollowFlag(false);
    if(s_cameraCom->cameraModeZ == CAMERA_LOCKED) return;
    setCameraModeZ(CameraManager::CAMERA_FREE);
    s_cameraCom->cameraZQueue.clear();
}

void CameraManager::clearCameraQueueR3d()
{
    //    setCameraFollowFlag(false);
    
    if(s_cameraCom->cameraModeR3d == CAMERA_LOCKED) return;
    setCameraModeR3d(CameraManager::CAMERA_FREE);
    s_cameraCom->cameraR3dQueue.clear();
}

void CameraManager::clearCameraQueueR2d()
{
    //    setCameraFollowFlag(false);
    
    if(s_cameraCom->cameraModeR2d == CAMERA_LOCKED) return;
    setCameraModeR2d(CameraManager::CAMERA_FREE);
    s_cameraCom->cameraR2dQueue.clear();
}

void CameraManager::clearCameraQueueRY()
{
    if(s_cameraCom->cameraModeRY == CAMERA_LOCKED) return;
    setCameraModeRY(CameraManager::CAMERA_FREE);
    s_cameraCom->cameraRYQueue.clear();
}


void CameraManager::addCameraQueueToX(float toX, float time, float delay, int nextCamState, float tweenMode)
{
    auto tarX = toX;
    float x = s_mainCamera->getCameraPosX();
    if (toX == -1) tarX = x;
    
    auto action = new BattleConfig::CameraActionData();
    action->toData = toX;
    
    action->time = time;
    action->delay = delay;
    action->elapse = 0;
    action->tweenMode = tweenMode;
    action->nextCameraState = nextCamState;
    
    s_cameraCom->cameraXQueue.push_back(action);
}

void CameraManager::addCameraQueueToY(float toY, float time, float delay, int nextCamState, float tweenMode, float backDelay)
{
    auto tarY = toY;
    float y = s_mainCamera->getCameraPosY();
    if (toY == -1) tarY = y;
    
    auto action = new BattleConfig::CameraActionData();
    action->toData = toY;
    
    action->time = time;
    action->delay = delay;
    action->elapse = 0;
    action->tweenMode = tweenMode;
    action->nextCameraState = nextCamState;
    
    s_cameraCom->cameraYQueue.push_back(action);
    s_cameraCom->backYDelay = backDelay;
}

void CameraManager::addCameraQueueToZ(float toZ, float time, float delay, int nextCamState, float tweenMode, float backDelay)
{
    auto tarZ = toZ;
    float z = s_mainCamera->getCameraPosZ();
    if (toZ == -1) tarZ = z;
    
    auto action = new BattleConfig::CameraActionData();
    action->toData = toZ;
    
    action->time = time;
    action->delay = delay;
    action->elapse = 0;
    action->tweenMode = tweenMode;
    action->nextCameraState = nextCamState;
    
    s_cameraCom->cameraZQueue.push_back(action);
    s_cameraCom->backZDelay = backDelay;
}

void CameraManager::addCameraQueueToR3d(float r3d, float time, float delay, int nextCamState, float tweenMode, float backDelay)
{
    auto tarRotate3d = r3d;
    float r = s_mainCamera->getCameraRotation3D();
    if (r3d == -1) tarRotate3d = r;
    
    auto action = new BattleConfig::CameraActionData();
    action->toData = r3d;
    
    action->time = time;
    action->delay = delay;
    action->elapse = 0;
    action->tweenMode = tweenMode;
    action->nextCameraState = nextCamState;
    
    s_cameraCom->cameraR3dQueue.push_back(action);
    s_cameraCom->backR3dDelay = backDelay;
}

void CameraManager::addCameraQueueToR2d(float r2d, float time, float delay, int nextCamState, float tweenMode, float backDelay)
{
    auto tarRotate2d = r2d;
    float r = s_mainCamera->getCameraRotation2D();
    if (r2d == -1) tarRotate2d = r;
    
    auto action = new BattleConfig::CameraActionData();
    action->toData = r2d;
    
    action->time = time;
    action->delay = delay;
    action->elapse = 0;
    action->tweenMode = tweenMode;
    action->nextCameraState = nextCamState;
    
    s_cameraCom->cameraR2dQueue.push_back(action);
    s_cameraCom->backR2dDelay = backDelay;
}

void CameraManager::addCameraQueueToRY(float rY, float time, float delay, int nextCamState, float tweenMode, float backDelay)
{
    auto tarRotateY = rY;
    float r = s_mainCamera->getCameraRotationY();
    if (rY == -1) tarRotateY = r;
    
    auto action = new BattleConfig::CameraActionData();
    action->toData = rY;
    
    action->time = time;
    action->delay = delay;
    action->elapse = 0;
    action->tweenMode = tweenMode;
    action->nextCameraState = nextCamState;
    
    s_cameraCom->cameraRYQueue.push_back(action);
    s_cameraCom->backRYDelay = backDelay;
}

void CameraManager::addCameraQueueTo(float toX, float toY, float toZ, float rotate, float time, float delay, int nextCamState, float tweenMode)
{
    auto tarX = toX;
    auto tarY = toY;
    auto tarZ = toZ;
    auto tarRotate = rotate;
    
    float x = s_mainCamera->getCameraPosX();
    float y = s_mainCamera->getCameraPosY();
    float z = s_mainCamera->getCameraPosZ();
    float r = s_mainCamera->getCameraRotation3D();
    
    if (toX == -1) tarX = x;
    if (toY == -1) tarY = y;
    if (toZ == -1) tarZ = z;
    if (rotate == -1) tarRotate = r;
    
    addCameraQueue(tarX, tarY, tarZ, tarRotate, time, delay, nextCamState, tweenMode);
}

void CameraManager::addCameraQueue(float toX, float toY, float toZ, float toR, float time, float delay, int nextCamState, float tweenMode)
{
    auto action = new BattleConfig::CameraAction();
    action->toX = toX;
    action->toY = toY;
    action->toZ = toZ;
    action->toRotate = toR;
    
    action->time = time;
    action->delay = delay;
    action->elapse = 0;
    
    action->tweenMode = tweenMode;
    
    action->nextCameraState = nextCamState;
    
    s_cameraCom->cameraQueue.push_back(action);
}

uint64_t CameraManager::getCameraCauseID()
{
    return s_cameraCom->causeId;
}

void CameraManager::setCameraCauseID(uint64_t id)
{
    // -3 smart s
    // -2 game over
    // -1 user control
    //  0 ani
    // >0 skill

    if (s_cameraCom->causeId != -2)
    {
        s_cameraCom->causeId = id;
    }
}

bool CameraManager::isInTwoCameraMode()
{
    return s_inTwoCamerasMode;
}

