//
//  SkillGuiCai.h
//  SGGame
//
//  Created by fuchenhao on 4/27/15.
//
//

#ifndef __Game__SkillGuiCai__
#define __Game__SkillGuiCai__

#include "SkillAction.h"
#include "BattleEvent.h"

class SkillGuiCai : public SkillAction, public entityx::Receiver<SkillGuiCai>
{
public:
    ~SkillGuiCai();
    
    void receive(const BattleEvent::HurtByAttack& e);
    
    void fireAction(entityx::EntityManager &es) override;

private:
    void killSoldierAtOnce(entityx::Entity entity);
    
    int m_damageRange = 0;
};

#endif /* defined(__Game__SkillGuiCai__) */
