//
//  SkillNuJi.h
//  sanguoGame
//
//  Created by fuchenhao on 7/30/15.
//
//

#ifndef __sanguoGame__SkillNuJi__
#define __sanguoGame__SkillNuJi__

#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SkillNuJi : public SkillAction, public entityx::Receiver<SkillNuJi>
{
public:
    ~SkillNuJi();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
};


#endif /* defined(__sanguoGame__SkillNuJi__) */
