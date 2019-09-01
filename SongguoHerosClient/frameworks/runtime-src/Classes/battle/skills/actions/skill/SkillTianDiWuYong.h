//
//  SkillTianDiWuYong.h
//  sanguoClient
//
//  Created by fuchenhao on 5/8/15.
//
//

#ifndef __sanguoClient__SkillTianDiWuYong__
#define __sanguoClient__SkillTianDiWuYong__

#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SkillTianDiWuYong : public SkillAction, public entityx::Receiver<SkillTianDiWuYong>
{
public:
    ~SkillTianDiWuYong();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
private:
    bool m_locked = false;
};

#endif /* defined(__sanguoClient__SkillTianDiWuYong__) */
