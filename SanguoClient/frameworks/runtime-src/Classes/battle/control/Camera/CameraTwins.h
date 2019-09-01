//
//  CameraTwins.h
//  SG
//
//  Created by fuchenhao on 15-3-3.
//
//

#ifndef __Game__CameraTwins__
#define __Game__CameraTwins__

#include <stdio.h>

#include "SGCamera.h"
#include "BattleConfig.h"

class CameraTwins
{
private:
    SGCamera* m_roleCamera;
    SGCamera* m_bgCamera;
    
    float m_cameraRotate3D;
    float m_cameraRotate2D;
    float m_cameraRotateY;
    
public:
    CameraTwins(SGCamera* roleCamera, SGCamera* bgCamera);
    virtual ~CameraTwins();
    
    float getCameraPosX();
    float getCameraPosY();
    float getCameraPosZ();
    
    void setCameraPosXTo(float x);
    void setCameraPosYTo(float y);
    void setCameraPosZTo(float z);

    float getCameraRotation3D();
    void setCameraRotationTo3D(float rotate);
    
    float getCameraRotation2D();
    void setCameraRotationTo2D(float rotate);
    
    float getCameraRotationY();
    void setCameraRotationToY(float rotate);
    
    void setCameraRotation3D();
};

#endif /* defined(__Game__CameraTwins__) */
