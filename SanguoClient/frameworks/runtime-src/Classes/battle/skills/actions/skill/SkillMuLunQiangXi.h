//
//  SkillMuLunQiangXi.h
//  sanguoClient
//
//  Created by fuchenhao on 4/20/15.
//
//

#ifndef __sanguoClient__SkillMuLunQiangXi__
#define __sanguoClient__SkillMuLunQiangXi__

#include "SkillAction.h"
#include "BattleEvent.h"

class SkillMuLunQiangXi : public SkillAction, public entityx::Receiver<SkillMuLunQiangXi>
{
public:
    ~SkillMuLunQiangXi();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::SkillArrowHitEvent& e);
    void receive(const BattleEvent::SkillArrowDestroyEvent& e);
    void receive(const BattleEvent::SkillArrowOutEvent& e);
};

#endif /* defined(__sanguoClient__SkillMuLunQiangXi__) */
