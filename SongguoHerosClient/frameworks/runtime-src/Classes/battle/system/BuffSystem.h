//
//  BuffSystem.h
//  Game
//
//  Created by fuchenhao on 3/3/15.
//
//

#ifndef __Game__BuffSystem__
#define __Game__BuffSystem__

#include "../../entityx/entityx.h"
#include "BattleEvent.h"
#include "BattleComponent.h"

class BuffSystem : public entityx::System<BuffSystem>, public entityx::Receiver<BuffSystem>
{
public:
    
    BuffSystem();
    
    void update(entityx::EntityManager& es, entityx::EventManager& events, double dt) override;

    void configure(entityx::EventManager& events) override;
    
    void receive(const BattleEvent::AddBuff& e);
    
    void receive(const BattleEvent::RemoveBuff& e);
    
    void receive(const BattleEvent::ClearAllBuff& e);
    
    void addBuff(const BattleConfig::BuffData& buffData);
    
    void removeBuff(int targetId, BattleConfig::Property targetProperty);
    
    void removeBuff(BattleConfig::BuffData* pBuffData, BattleComponent::Buff::Handle& buff, BattleComponent::Property::Handle& property);
    
    bool hasLockBuff(BattleConfig::Property targetProperty, BattleComponent::Buff::Handle& buff);
    
};

#endif /* defined(__Game__BuffSystem__) */
