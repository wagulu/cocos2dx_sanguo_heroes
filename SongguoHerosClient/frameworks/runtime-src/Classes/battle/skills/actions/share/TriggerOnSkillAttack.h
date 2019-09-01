//
//  TriggerOnSkillAttack.h
//  sanguoGame
//
//  Created by fuchenhao on 7/31/15.
//
//

#ifndef __sanguoGame__TriggerOnSkillAttack__
#define __sanguoGame__TriggerOnSkillAttack__

#include "SkillAction.h"
#include "BattleEvent.h"

class TriggerOnSkillAttack : public SkillAction, public entityx::Receiver<TriggerOnSkillAttack>
{
public:
    ~TriggerOnSkillAttack();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
};

#endif /* defined(__sanguoGame__TriggerOnSkillAttack__) */
