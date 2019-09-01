//
//  SkillHongLianYeHuo.h
//  sanguoClient
//
//  Created by fuchenhao on 6/19/15.
//
//

#ifndef __sanguoClient__SkillHongLianYeHuo__
#define __sanguoClient__SkillHongLianYeHuo__


#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SkillHongLianYeHuo : public SkillAction, public entityx::Receiver<SkillHongLianYeHuo>
{
public:
    ~SkillHongLianYeHuo();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
};


#endif /* defined(__sanguoClient__SkillHongLianYeHuo__) */
