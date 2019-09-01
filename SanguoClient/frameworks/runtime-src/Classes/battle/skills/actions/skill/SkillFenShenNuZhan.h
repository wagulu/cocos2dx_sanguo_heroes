//
//  ShowMoveEffect.h
//  Game
//
//  Created by zhaikaiyu on 3/9/15.
//
//

#ifndef __SHOW_MOVE_EFFECT__
#define __SHOW_MOVE_EFFECT__

#include "SkillAction.h"
#include "BattleEvent.h"

class SkillFenShenNuZhan : public SkillAction, public entityx::Receiver<SkillFenShenNuZhan>
{
public:
    ~SkillFenShenNuZhan();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::SkillArrowHitEvent& e);
    void receive(const BattleEvent::SkillArrowDestroyEvent& e);
    void receive(const BattleEvent::SkillArrowOutEvent& e);
    void receive(const BattleEvent::SkillArrowOutOfHpEvent& e);

private:
    void fireFinalHitAtPosition(float z, float x);
    
    bool m_finalHitFired = false;
};

#endif /* defined(__SHOW_MOVE_EFFECT__) */
