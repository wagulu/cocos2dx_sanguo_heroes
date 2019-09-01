//
//  ArrowSystem.h
//  Game
//
//  Created by fuchenhao on 12/22/14.
//
//

#ifndef __ARROW_SYSTEM_H__
#define __ARROW_SYSTEM_H__

#include "../../entityx/entityx.h"

#include "BattleConfig.h"
#include "BattleEvent.h"
#include "BattleWorld.h"
#include "BattleComponent.h"

//#define START_TRACK_TILE     2
//#define TRACK_SPEED_FACTOR   2.5

class ArrowSystem : public entityx::System<ArrowSystem>, public entityx::Receiver<ArrowSystem>
{
public:
    void configure(entityx::EventManager& events);
    void receive(const BattleEvent::AddSkillArrow& e);
    void receive(const BattleEvent::BattlePause& e);
    void receive(const BattleEvent::BattleResume& e);
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
private:
    bool m_isPausing = false;
};

#endif
