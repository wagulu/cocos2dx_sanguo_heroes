//
//  ActorModeSystem.h
//  Game
//
//  Created by fu.chenhao on 3/5/15.
//
//

#ifndef __Game__ActorModeSystem__
#define __Game__ActorModeSystem__

#include <stdio.h>
#include "../../entityx/entityx.h"
#include "BattleEvent.h"

#endif /* defined(__Game__ActorModeSystem__) */

class ActorModeSystem : public entityx::System<ActorModeSystem>, public entityx::Receiver<ActorModeSystem>
{
public:
    void configure(entityx::EventManager& events) override;
    void update(entityx::EntityManager& es, entityx::EventManager& events, double dt) override;

    void receive(const BattleEvent::SetActorMode& e);

};
