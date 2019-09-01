//
//  GasSystem.h
//  Game
//
//  Created by zhaikaiyu on 3/2/15.
//
//

#ifndef __Game__GasSystem__
#define __Game__GasSystem__

#include "BattleConfig.h"
#include "cocos2d.h"
#include "BattleEvent.h"
#include "SkillManager.h"

class GasSystem : public entityx::System<GasSystem>, public entityx::Receiver<GasSystem>
{
public:
    void configure(entityx::EventManager& events) override;
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    void receive(const BattleEvent::ChangeGasEvent& e);
    void receive(const BattleEvent::PreFireSkill& e);
    void receive(const BattleEvent::LockSkill& e);
    void receive(const BattleEvent::UnlockSkill& e);
    
private:
    void recoverEnergy(entityx::Entity& entity, float value);
    void emitLockStatusChange();
    
    bool m_skillLockLeft = false;
    bool m_skillLockRight = false;
};

#endif /* defined(__Game__GasSystem__) */
