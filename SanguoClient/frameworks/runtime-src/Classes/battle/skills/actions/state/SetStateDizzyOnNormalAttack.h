//
//  SetStateDizzyOnNormalAttack.h
//  sanguoClient
//
//  Created by fuchenhao on 4/23/15.
//
//

#ifndef __sanguoClient__SetStateDizzyOnNormalAttack__
#define __sanguoClient__SetStateDizzyOnNormalAttack__

#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SetStateDizzyOnNormalAttack : public SetStateDizzyBase, public entityx::Receiver<SetStateDizzyOnNormalAttack>
{
public:
    ~SetStateDizzyOnNormalAttack();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
};

#endif /* defined(__sanguoClient__SetStateDizzyOnNormalAttack__) */
