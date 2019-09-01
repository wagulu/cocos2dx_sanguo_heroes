//
//  AddHpOnReduceEnemyHp.h
//  sanguoGame
//
//  Created by fuchenhao on 8/26/15.
//
//

#ifndef __sanguoGame__AddHpOnReduceEnemyHp__
#define __sanguoGame__AddHpOnReduceEnemyHp__

#include "SkillAction.h"
#include "BattleEvent.h"

class AddHpOnReduceEnemyHp : public SkillAction, public entityx::Receiver<AddHpOnReduceEnemyHp>
{
public:
    ~AddHpOnReduceEnemyHp();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
    void receive(const BattleEvent::HurtByAttack& e);

private:
    float m_hpAdded = 0;
    float m_delay = 0;
};

#endif /* defined(__sanguoGame__AddHpOnReduceEnemyHp__) */
