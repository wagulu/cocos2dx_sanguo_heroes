//
//  AnimationSystem.h
//  Game
//
//  Created by fuchenhao on 3/16/15.
//
//


#ifndef __ANIMATION_SYSTEM_H__
#define __ANIMATION_SYSTEM_H__

#include "../../entityx/entityx.h"

#include "BattleComponent.h"
#include "BattleEvent.h"
#include "BattleWorld.h"

class AnimationSystem : public entityx::System<AnimationSystem>, public entityx::Receiver<AnimationSystem>
{
public:
    
    void configure(entityx::EventManager& events) override;
    
    void receive(const BattleEvent::BattlePause& e);
    void receive(const BattleEvent::BattleResume& e);
    void receive(const BattleEvent::AddEntityAnimation& e);
    void receive(const BattleEvent::AddBuffAnimation& e);
    void receive(const BattleEvent::RemoveEntityAnimation& e);
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
private:
    
    bool m_isPausing = false;
    
    void applyTweenOperation(TweenQueue::TARGET target, float value, BattleAnimation* anim);
};

#endif /* defined(__ANIMATION_SYSTEM_H__) */
