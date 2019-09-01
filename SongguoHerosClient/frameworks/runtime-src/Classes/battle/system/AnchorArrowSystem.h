//
//  AnchorArrowSystem.h
//  sanguoClient
//
//  Created by fu.chenhao on 4/16/15.
//
//

#ifndef __sanguoClient__AnchorArrowSystem__
#define __sanguoClient__AnchorArrowSystem__

#include <stdio.h>
#include "../../entityx/entityx.h"
#include "BattleEvent.h"

class AnchorArrowSystem : public entityx::System<AnchorArrowSystem>, public entityx::Receiver<AnchorArrowSystem>
{
public:
    void configure(entityx::EventManager& events) override;
    void update(entityx::EntityManager& es, entityx::EventManager& events, double dt) override;
    
private:
    entityx::Entity* checkHurtEnemy(entityx::Entity firer, float ox, float cx, float z);
    void doAttack(entityx::Entity& firer, entityx::Entity& target);

};

#endif /* defined(__sanguoClient__AnchorArrowSystem__) */
