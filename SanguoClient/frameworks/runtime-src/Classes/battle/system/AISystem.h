//
//  AISystem.h
//  Game
//
//  Created by fuchenhao on 3/11/15.
//
//

#ifndef __AI_SYSTEM__
#define __AI_SYSTEM__

#include "../../entityx/entityx.h"

#include "BattleComponent.h"
#include "BattleEvent.h"
#include "BattleWorld.h"
#include "AIAction.h"
#include "AICondition.h"
#include "AIManager.h"
#include "cocos2d.h"

class AISystem : public entityx::System<AISystem>, public entityx::Receiver<AISystem>
{
public:
    ~AISystem();
    
    void configure(entityx::EventManager& events) override;

    void receive(const BattleEvent::AddAI& e);

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    void startDelegate(cocos2d::EventCustom* event);
    void stopDelegate(cocos2d::EventCustom* event);

};

#endif /* defined(__Game__AISystem__) */
