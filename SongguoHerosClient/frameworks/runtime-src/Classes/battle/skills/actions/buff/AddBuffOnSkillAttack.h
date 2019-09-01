//
//  AddBuffOnSkillAttack.h
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#ifndef __sanguoClient__AddBuffOnSkillAttack__
#define __sanguoClient__AddBuffOnSkillAttack__

#include "SkillAction.h"
#include "BattleEvent.h"
#include "AddBuffBase.h"

class AddBuffOnSkillAttack : public AddBuffBase, public entityx::Receiver<AddBuffOnSkillAttack>
{
public:
    ~AddBuffOnSkillAttack();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
    
private:
    bool m_onlyOnce = false;
};

#endif /* defined(__sanguoClient__AddBuffOnSkillAttack__) */
