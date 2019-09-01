//
//  DelayTrigger.h
//  sanguoGame
//
//  Created by fuchenhao on 7/17/15.
//
//

#ifndef __sanguoGame__DelayTrigger__
#define __sanguoGame__DelayTrigger__

#include "SkillAction.h"

class DelayTrigger : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
private:
    float m_elapse = 0;
    float m_delay;
};



#endif /* defined(__sanguoGame__DelayTrigger__) */
