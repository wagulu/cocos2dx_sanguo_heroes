//
//  HeroEventSystem.h
//  Game
//
//  Created by fu.chenhao on 3/2/15.
//
//

#ifndef __Game__HeroEventSystem__
#define __Game__HeroEventSystem__

#include "entityx.h"
#include "BattleEvent.h"

class HeroEventSystem : public entityx::System<HeroEventSystem>, public entityx::Receiver<HeroEventSystem>
{
public:
    void configure(entityx::EventManager& events) override;
    
    void receive(const BattleEvent::BeAttacked& e);

    void receive(const BattleEvent::BattleActorDead& e);
    
    void receive(const BattleEvent::StrategyBattleEnd& e);

    void receive(const BattleEvent::StrategyHeroRush& e);
    
    void receive(const BattleEvent::StrategyHeroStop& e);
    
    void receive(const BattleEvent::StrategyHeroPlayAnimation& e);
    
    void receive(const BattleEvent::StrategyHeroDash& e);
    
    void receive(const BattleEvent::StrategyHeroDizzy& e);
    
    void receive(const BattleEvent::StrategyHeroMoveToTargetTile& e);
    
    void update(entityx::EntityManager& es, entityx::EventManager& events, double dt) override;
};

#endif /* defined(__Game__HeroEventSystem__) */
