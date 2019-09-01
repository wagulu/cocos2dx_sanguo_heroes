//
//  EventSystem.h
//  Game
//
//  Created by fu.chenhao on 3/2/15.
//
//

#ifndef __Game__SoldierEventSystem__
#define __Game__SoldierEventSystem__

#include <stdio.h>
#include "entityx.h"
#include "BattleEvent.h"

#endif /* defined(__Game__EventSystem__) */

class SoldierEventSystem : public entityx::System<SoldierEventSystem>, public entityx::Receiver<SoldierEventSystem>
{
public:
    void configure(entityx::EventManager& events) override;
    void update(entityx::EntityManager& es, entityx::EventManager& events, double dt) override;
    
//    void receive(const BattleEvent::ForceChangeState& e);
    
    void receive(const BattleEvent::BeAttacked& e);
    
    void receive(const BattleEvent::StrategyBattleEnd& e);
    void receive(const BattleEvent::StrategyIdle& e);
    void receive(const BattleEvent::StrategyAttack& e);
    void receive(const BattleEvent::StrategyProtectHero& e);
    void receive(const BattleEvent::StrategyRecenter& e);
    void receive(const BattleEvent::StrategyDepart& e);
    void receive(const BattleEvent::StrategyAttackHero& e);
    void receive(const BattleEvent::StrategyMoveBack& e);
    void receive(const BattleEvent::StrategySoldierDizzy& e);
    void receive(const BattleEvent::StrategySoldierFadeIn& e);
    void receive(const BattleEvent::StrategySoldierChangeToGhost& e);
    void receive(const BattleEvent::StrategySoldierChangeToBlock& e);
    void receive(const BattleEvent::StateSoldierPlayAnimation& e);
    void receive(const BattleEvent::StateSoldierHitBack& e);

    void receive(const BattleEvent::BattleActorDead& e);
    void receive(const BattleEvent::HeroPushBattleActor& e);

//    void receive(const BattleEvent::SwitchActorMode& e);

    void receive(const BattleEvent::KnockBackSoldier& e);

};
