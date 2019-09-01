//
//  TickerSystem.h
//  sanguoClient
//
//  Created by fu.chenhao on 4/24/15.
//
//

#ifndef __sanguoClient__TickerSystem__
#define __sanguoClient__TickerSystem__

#include <stdio.h>
#include "../../entityx/entityx.h"

#include "BattleComponent.h"
#include "BattleEvent.h"
#include "BattleWorld.h"

class TickerSystem : public entityx::System<TickerSystem>
{
public:
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
private:
    
    
    
};

#endif /* defined(__sanguoClient__TickerSystem__) */
