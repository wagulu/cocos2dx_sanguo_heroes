//
//  CameraEventSystem.h
//  SG
//
//  Created by fuchenhao on 15-3-3.
//
//

#ifndef __Game__CameraEventSystem__
#define __Game__CameraEventSystem__

#include <stdio.h>
#include "BattleEvent.h"
#include "CameraManager.h"
#include "BattleWorld.h"

using namespace entityx;

class CameraEventSystem : public System<CameraEventSystem>, public Receiver<CameraEventSystem>
{
private:
    bool m_shakeDisable = false;
    
public:
    void configure(EventManager& events) override;
    
    void receive(const BattleEvent::CameraMoveToEvent& e);
    
    void receive(const BattleEvent::CameraMoveToWithAniEvent& e);
    void receive(const BattleEvent::CameraMoveToWithAniXEvent& e);
    void receive(const BattleEvent::CameraMoveToWithAniYEvent& e);
    void receive(const BattleEvent::CameraMoveToWithAniZEvent& e);
    void receive(const BattleEvent::CameraMoveToWithAniR3dEvent& e);
    void receive(const BattleEvent::CameraMoveToWithAniR2dEvent& e);
    void receive(const BattleEvent::CameraMoveToWithAniRYEvent& e);

    void receive(const BattleEvent::CameraFollowEntity& e);
    void receive(const BattleEvent::CameraTriggerFollowEntity& e);
    
    void receive(const BattleEvent::CameraShakeEvent& e);
    void receive(const BattleEvent::CameraDisableShakeEvent& e);
    void receive(const BattleEvent::CameraClearShakeEvent& e);
    
    void receive(const BattleEvent::CameraQueueClearEvent& e);
    
    void update(EntityManager& es, EventManager& events, double dt) override;
};

#endif /* defined(__Game__CameraEventSystem__) */
