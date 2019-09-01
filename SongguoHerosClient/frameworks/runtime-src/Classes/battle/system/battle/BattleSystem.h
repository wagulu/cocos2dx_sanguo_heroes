//
//  BattleSystem.h
//  sanguoGame
//
//  Created by fuchenhao on 7/13/15.
//
//

#ifndef __sanguoGame__BattleSystem__
#define __sanguoGame__BattleSystem__

#include "entityx.h"
#include "BattleEvent.h"

class BattleSystem : public entityx::System<BattleSystem>, public entityx::Receiver<BattleSystem>
{
public:
    enum BattleState
    {
        BEFORE_START,
        START_FIGHT,
        CHANGE_BACKUP_HERO,
        BATTLE_END
    };
    
    ~BattleSystem();
    
    void configure(entityx::EventManager& events) override;
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    void receive(const BattleEvent::StartBattleComplete &e);
    
    void receive(const BattleEvent::BattleActorDead& e);
    
    void receive(const BattleEvent::ExitBattleBeforeEnd& e);
    
private:
    float m_backToDefaultTimeRateDelay = 0;
};

#endif /* defined(__sanguoGame__BattleSystem__) */
