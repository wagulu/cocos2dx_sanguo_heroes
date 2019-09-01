//
//  CurveArrow.h
//  sanguoGame
//
//  Created by fuchenhao on 7/28/15.
//
//

#ifndef __sanguoGame__CurveArrow__
#define __sanguoGame__CurveArrow__


#include "SkillAction.h"
#include "BattleEvent.h"

class SkillArrow : public SkillAction, public entityx::Receiver<SkillArrow>
{
public:
    ~SkillArrow();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::SkillArrowHitEvent& e);
    void receive(const BattleEvent::SkillArrowDestroyEvent& e);
};


#endif /* defined(__sanguoGame__CurveArrow__) */
