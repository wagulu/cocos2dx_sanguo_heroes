//
//  ComboSystem.h
//  Game
//
//  Created by fu.chenhao on 3/24/15.
//
//

#ifndef __Game__ComboSystem__
#define __Game__ComboSystem__

#include <stdio.h>
#include "../../entityx/entityx.h"
#include "BattleEvent.h"

class ComboSystem : public entityx::System<ComboSystem>, public entityx::Receiver<ComboSystem>
{
public:
    void configure(entityx::EventManager& events) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
    
    void update(entityx::EntityManager& es, entityx::EventManager& events, double dt) override;
    

};

#endif /* defined(__Game__ComboSystem__) */
