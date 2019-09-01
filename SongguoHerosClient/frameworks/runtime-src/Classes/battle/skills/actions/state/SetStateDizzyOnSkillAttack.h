//
//  SetStateDizzyOnSkillAttack.h
//  sanguoClient
//
//  Created by fuchenhao on 4/23/15.
//
//

#ifndef __sanguoClient__SetStateDizzyOnSkillAttack__
#define __sanguoClient__SetStateDizzyOnSkillAttack__

#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SetStateDizzyOnSkillAttack : public SetStateDizzyBase, public entityx::Receiver<SetStateDizzyOnSkillAttack>
{
public:
    ~SetStateDizzyOnSkillAttack();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
};

#endif /* defined(__sanguoClient__SetStateDizzyOnSkillAttack__) */
