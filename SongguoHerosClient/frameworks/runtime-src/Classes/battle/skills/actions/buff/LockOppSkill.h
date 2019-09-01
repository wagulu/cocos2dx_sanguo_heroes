//
//  LockSkill.h
//  sanguoClient
//
//  Created by fuchenhao on 5/7/15.
//
//

#ifndef __sanguoClient__LockOppSkill__
#define __sanguoClient__LockOppSkill__

#include "SkillAction.h"

class LockOppSkill : public SkillAction
{
public:
    ~LockOppSkill();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
private:
    void setLockState(bool lock);
    
    float m_lifeTime;
    bool m_hasLocked = false;
};

#endif /* defined(__sanguoClient__LockOppSkill__) */
