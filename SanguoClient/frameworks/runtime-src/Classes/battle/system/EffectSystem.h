//
//  EffectSystem.h
//  Game
//
//  Created by fuchenhao on 12/22/14.
//
//

#ifndef __EFFECT_SYSTEM_H__
#define __EFFECT_SYSTEM_H__

#include "../../entityx/entityx.h"

#include "BattleConfig.h"
#include "BattleEvent.h"
#include "BattleWorld.h"
#include "BattleComponent.h"

class EffectSystem : public entityx::System<EffectSystem>, public entityx::Receiver<EffectSystem>
{
public:
    void configure(entityx::EventManager& events) override;

    void receive(const BattleEvent::AddEffectToEntity& e);
    void receive(const BattleEvent::AddEffect& e);
    void receive(const BattleEvent::AddEffectSimple& e);
    void receive(const BattleEvent::AddEffectFlyText& e);
    void receive(const BattleEvent::BattlePause& e);
    void receive(const BattleEvent::BattleResume& e);

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
private:
    bool m_isPausing = false;
};


#endif
