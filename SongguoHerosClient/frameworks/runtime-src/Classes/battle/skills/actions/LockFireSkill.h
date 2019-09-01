//
//  LockFireSkill.h
//  sanguoClient
//
//  Created by fuchenhao on 6/3/15.
//
//

#ifndef __sanguoClient__LockFireSkill__
#define __sanguoClient__LockFireSkill__

#include "SkillAction.h"

class LockFireSkill : public SkillAction
{
public:
    ~LockFireSkill();
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    void onTriggerFired(const std::string& trigger);
    
private:
    void unlockFireSkill();
    
    float m_unlockDelay = 0;
    std::string* m_pUnlockTrigger = nullptr;
    bool m_unlockRequired = false;
};

#endif /* defined(__sanguoClient__DisableFireSkill__) */
