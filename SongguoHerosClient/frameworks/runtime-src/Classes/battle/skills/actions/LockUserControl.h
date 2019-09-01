//
//  LockUserControl.h
//  sanguoClient
//
//  Created by fuchenhao on 6/8/15.
//
//

#ifndef __sanguoClient__LockUserControl__
#define __sanguoClient__LockUserControl__

#include "SkillAction.h"
#include "BattleEvent.h"

//#include "SimpleTween.h"

class LockUserControl : public SkillAction, public entityx::Receiver<BattleEvent::SetCameraModeForLockEvent>
{
public:
    ~LockUserControl();
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    void onTriggerFired(const std::string& trigger);
    
    void receive(const BattleEvent::SetCameraModeForLockEvent& e);
    
private:
    void unlockUserControl();
    void resetCamera();
    
    int m_preCameraMode = -1;
    int m_cameraFollowTargetId = -1;

    float m_unlockDelay = 0;
    float m_elapse = 0;
    bool m_autoBack = true;
    
    std::string* m_pUnlockTrigger = nullptr;
    bool m_unlockRequired = false;
    
    SimpleTween_linear* m_timeTween = nullptr;
    SimpleTween_linear* m_timeTween2 = nullptr;
};

#endif /* defined(__sanguoClient__LockUserControl__) */
