//
//  CameraTwins.cpp
//  SG
//
//  Created by fuchenhao on 15-3-3.
//
//

#include "CameraTwins.h"
#include "CameraManager.h"

CameraTwins::CameraTwins(SGCamera* roleCamera, SGCamera* bgCamera)
{
    m_roleCamera = roleCamera;
    m_bgCamera = bgCamera;
    
    m_cameraRotate3D = CameraManager::INIT_CAMERA_ROTATE;
    m_cameraRotate2D = CameraManager::INIT_CAMERA_ROTATE_2D;
    m_cameraRotateY  = CameraManager::INIT_CAMERA_ROTATE_Y;
}

CameraTwins::~CameraTwins()
{
    
}


// pos
float CameraTwins::getCameraPosX()
{
    if(m_roleCamera == nullptr)
    {
        return 0;
    }
    
    return m_roleCamera->getPositionX();
}

float CameraTwins::getCameraPosY()
{
    if(m_roleCamera == nullptr)
    {
        return 0;
    }
    
    return m_roleCamera->getPositionY();
}

float CameraTwins::getCameraPosZ()
{
    if(m_roleCamera == nullptr)
    {
        return 0;
    }
    
    return m_roleCamera->getPositionZ();
}

void CameraTwins::setCameraPosXTo(float x)
{
    m_roleCamera->setPositionX(x);
    m_bgCamera->setPositionX(x);
}

void CameraTwins::setCameraPosZTo(float z)
{
    m_roleCamera->setPositionZ(z);
    m_bgCamera->setPositionZ(z);
}

void CameraTwins::setCameraPosYTo(float y)
{
    m_roleCamera->setPositionY(y);
    m_bgCamera->setPositionY(y);
}


float CameraTwins::getCameraRotation3D()
{
    return m_cameraRotate3D;
}


void CameraTwins::setCameraRotation3D()
{
    m_roleCamera->setRotation3D(cocos2d::Vec3(m_cameraRotate3D, m_cameraRotateY, m_cameraRotate2D));
    m_bgCamera->setRotation3D(cocos2d::Vec3(m_cameraRotate3D, m_cameraRotateY, m_cameraRotate2D));
}


void CameraTwins::setCameraRotationTo3D(float rotate)
{
    m_cameraRotate3D = rotate;
    setCameraRotation3D();
}

float CameraTwins::getCameraRotation2D()
{
    return m_cameraRotate2D;
}

void CameraTwins::setCameraRotationTo2D(float rotate)
{
//    CCLOG("============ setCameraRotationTo2");
    m_cameraRotate2D = rotate;
    setCameraRotation3D();
//    m_roleCamera->setRotation(m_cameraRotate2D);
//    m_bgCamera->setRotation(m_cameraRotate2D);
}

float CameraTwins::getCameraRotationY()
{
    return m_cameraRotateY;
}

void CameraTwins::setCameraRotationToY(float rotate)
{
    //    CCLOG("============ setCameraRotationTo2");
    m_cameraRotateY = rotate;
    setCameraRotation3D();
}

