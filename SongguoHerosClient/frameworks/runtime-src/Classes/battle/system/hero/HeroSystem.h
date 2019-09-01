//
//  HeroSystem.h
//  Game
//
//  Created by fu.chenhao on 2/26/15.
//
//

#ifndef __Game__HeroSystem__
#define __Game__HeroSystem__

#include "BattleConfig.h"
#include "BattleEvent.h"
#include "BattleComponent.h"
#include "cocos2d.h"


static bool isHeroStrategyState(int objectStateCode)
{
    return(
           objectStateCode == BattleConfig::GENERAL_RUN
           || objectStateCode == BattleConfig::GENERAL_IDLE
           );
    
}

static bool isHeroModeState(int objectStateCode)
{
    return(
           objectStateCode == BattleConfig::GENERAL_DIZZY
           );
    
}

class HeroSystem : public entityx::System<HeroSystem>, public entityx::Receiver<HeroSystem>
{
public:
    
    void configure(entityx::EventManager& events) override;
    void receive(const BattleEvent::BattlePause& e);
    void receive(const BattleEvent::BattleResume& e);

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
private:
    
    bool m_isPausing = false;
};

#endif /* defined(__Game__HeroSystem__) */
