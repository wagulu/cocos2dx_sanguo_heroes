//
//  SkillNiePan.h
//  sanguoClient
//
//  Created by fuchenhao on 4/27/15.
//
//

#ifndef __sanguoClient__SkillNiePan__
#define __sanguoClient__SkillNiePan__


#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SkillNiePan : public SkillAction, public entityx::Receiver<SkillNiePan>
{
public:
    ~SkillNiePan();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
private:
    float m_startX;
};

#endif /* defined(__sanguoClient__SkillNiePan__) */
