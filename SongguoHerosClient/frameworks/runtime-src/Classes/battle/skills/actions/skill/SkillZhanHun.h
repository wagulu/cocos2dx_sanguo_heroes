//
//  SkillZhanHun.h
//  sanguoClient
//
//  Created by fuchenhao on 4/27/15.
//
//

#ifndef __sanguoClient__SkillZhanHun__
#define __sanguoClient__SkillZhanHun__

#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SkillZhanHun : public SkillAction, public entityx::Receiver<SkillZhanHun>
{
public:
    ~SkillZhanHun();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
};

#endif /* defined(__sanguoClient__SkillZhanHun__) */
