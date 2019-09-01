//
//  AddEffectOnAddNormalAttack.h
//  sanguoGame
//
//  Created by fuchenhao on 8/27/15.
//
//

#ifndef __sanguoGame__AddEffectOnAddNormalAttack__
#define __sanguoGame__AddEffectOnAddNormalAttack__

#include "SkillAction.h"
#include "BattleEvent.h"

class AddEffectOnAddNormalAttack : public SkillAction, public entityx::Receiver<AddEffectOnAddNormalAttack>
{
public:
    ~AddEffectOnAddNormalAttack();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
    void receive(const BattleEvent::AddAttack& e);
    
private:
    float m_duration = 0;
    std::string m_effectName;
    float m_effectDelay = 0;
    float m_effectDuration = 0;
};

#endif /* defined(__sanguoGame__AddEffectOnAddNormalAttack__) */
