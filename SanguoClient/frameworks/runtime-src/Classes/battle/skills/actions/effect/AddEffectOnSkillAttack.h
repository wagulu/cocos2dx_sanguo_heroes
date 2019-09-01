//
//  AddEffectOnSkillAttack.h
//  sanguoClient
//
//  Created by fuchenhao on 5/29/15.
//
//

#ifndef __sanguoClient__AddEffectOnSkillAttack__
#define __sanguoClient__AddEffectOnSkillAttack__

#include "SkillAction.h"
#include "BattleEvent.h"

class AddEffectOnSkillAttack : public SkillAction, public entityx::Receiver<AddEffectOnSkillAttack>
{
public:
    ~AddEffectOnSkillAttack();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
};

#endif /* defined(__sanguoClient__AddEffectOnSkillAttack__) */
