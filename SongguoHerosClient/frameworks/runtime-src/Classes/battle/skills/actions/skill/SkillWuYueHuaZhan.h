//
//  WuYueHuaZhan.h
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#ifndef __sanguoClient__WuYueHuaZhan__
#define __sanguoClient__WuYueHuaZhan__

#include "SkillAction.h"
#include "BattleEvent.h"

class SkillWuYueHuaZhan : public SkillAction, public entityx::Receiver<SkillWuYueHuaZhan>
{
public:
    ~SkillWuYueHuaZhan();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::SkillArrowHitEvent& e);
    void receive(const BattleEvent::SkillArrowDestroyEvent& e);
    void receive(const BattleEvent::SkillArrowOutEvent& e);
private:
    int m_total = 5;
};

#endif /* defined(__sanguoClient__WuYueHuaZhan__) */
