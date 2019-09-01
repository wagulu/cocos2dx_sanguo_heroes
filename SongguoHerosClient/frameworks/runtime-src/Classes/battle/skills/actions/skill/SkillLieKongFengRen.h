//
//  SkillLieKongFengRen.h
//  sanguoClient
//
//  Created by fuchenhao on 4/10/15.
//
//

#ifndef __sanguoClient__SkillLieKongFengRen__
#define __sanguoClient__SkillLieKongFengRen__

#include "SkillAction.h"
#include "BattleEvent.h"

class SkillLieKongFengRen : public SkillAction, public entityx::Receiver<SkillLieKongFengRen>
{
public:
    ~SkillLieKongFengRen();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::SkillArrowHitEvent& e);
    void receive(const BattleEvent::SkillArrowDestroyEvent& e);
    void receive(const BattleEvent::SkillArrowOutEvent& e);
};

#endif /* defined(__sanguoClient__SkillLieKongFengRen__) */
