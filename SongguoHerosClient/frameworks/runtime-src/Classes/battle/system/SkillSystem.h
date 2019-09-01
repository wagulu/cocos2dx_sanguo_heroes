//
//  SkillSystem.h
//  Game
//
//  Created by fuchenhao on 2/9/15.
//
//


#ifndef __SKILL_SYSTEM_H__
#define __SKILL_SYSTEM_H__

#include "entityx.h"
#include "cocos2d.h"

#include "BattleComponent.h"
#include "BattleEvent.h"
#include "BattleWorld.h"
#include "SkillManager.h"

using namespace cocos2d;

class SkillSystem : public entityx::System<SkillSystem>, public entityx::Receiver<SkillSystem>
{
public:
    void configure(entityx::EventManager& events) override;

    void receive(const BattleEvent::FireSkill& e);
    
    void receive(const BattleEvent::FireNegativeSkill& e);
    
    void receive(const BattleEvent::CancelSkill& e);

    void receive(const BattleEvent::TriggerSkillAction& e);
    
    void receive(const BattleEvent::BattlePause& e);
    
    void receive(const BattleEvent::BattleResume& e);

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    ~SkillSystem();
    
private:
    bool m_isPausing = false;
};

#endif
