//
//  SGCamera.cpp
//  SGCamera
//
//  Created by fuchenhao on 1/4/15.
//
//

#include "SGCamera.h"

SGCamera* SGCamera::create()
{
    SGCamera* camera = new (std::nothrow) SGCamera();
    camera->initDefault();
    camera->autorelease();
    
    return camera;
}

SGCamera* SGCamera::createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
    auto ret = new (std::nothrow) SGCamera();
    if (ret)
    {
        ret->initPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
