//
//  SkillChuanSuo.h
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#ifndef __sanguoGame__SkillChuanSuo__
#define __sanguoGame__SkillChuanSuo__

#include "SkillAction.h"
#include "BattleEvent.h"

class SkillChuanSuo : public SkillAction, public entityx::Receiver<SkillChuanSuo>
{
public:
    ~SkillChuanSuo();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::SkillArrowHitEvent& e);
    void receive(const BattleEvent::SkillArrowDestroyEvent& e);
};

#endif /* defined(__sanguoGame__SkillChuanSuo__) */
