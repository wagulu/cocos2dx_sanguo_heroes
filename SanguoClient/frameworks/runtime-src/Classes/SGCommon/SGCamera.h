//
//  SGCamera.h
//  SGCamera
//
//  Created by fuchenhao on 1/4/15.
//
//

#ifndef __SG_CAMERA_H__
#define __SG_CAMERA_H__

#include "cocos2d.h"

class SGCamera : public cocos2d::Camera
{
public:
    static SGCamera* create();
    static SGCamera* createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
};

#endif /* defined(__SG_CAMERA_H__) */
