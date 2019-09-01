//
//  EmptyArrow.h
//  sanguoGame
//
//  Created by fuchenhao on 7/22/15.
//
//

#ifndef __sanguoGame__EmptyArrow__
#define __sanguoGame__EmptyArrow__

#include "SkillAction.h"
#include "BattleEvent.h"

class EmptyArrow : public SkillAction, public entityx::Receiver<EmptyArrow>
{
public:
    ~EmptyArrow();
    
    void receive(const BattleEvent::SkillArrowHitEvent& e);
    void receive(const BattleEvent::SkillArrowDestroyEvent& e);
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoGame__EmptyArrow__) */
