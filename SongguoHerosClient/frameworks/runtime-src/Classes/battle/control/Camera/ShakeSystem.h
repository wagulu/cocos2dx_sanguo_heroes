//
//  ShakeSystem.h
//  SG
//
//  Created by fuchenhao on 3/16/15.
//
//

#ifndef __SHAKE_SYSTEM_H__
#define __SHAKE_SYSTEM_H__


#include "BattleConfig.h"
#include "BattleComponent.h"
#include "BattleWorld.h"

#include "CameraManager.h"

#define MAX_SHAKE_H     200
#define MAX_SHAKE_V     200

class ShakeSystem : public entityx::System<ShakeSystem>
{
    /*
private:
    void setTargetOrgPos(BattleComponent::ShakeCom::Handle shake);
    
    std::vector<cocos2d::Node*> targets;
    void removeTarget(cocos2d::Node* shakeTarget);*/
    
public:
    enum ShakeMode
    {
        SHAKE_NONE,
        SHAKE_X,
        SHAKE_Y,
        SHAKE_Z,
        SHAKE_BOTH_XY,
        SHAKE_BOTH_XYZ
    };
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
};


#endif
