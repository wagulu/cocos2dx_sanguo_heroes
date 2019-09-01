//
//  SkillLianHuan.h
//  sanguoGame
//
//  Created by fuchenhao on 7/29/15.
//
//

#ifndef __sanguoGame__SkillLianHuan__
#define __sanguoGame__SkillLianHuan__

#include "SkillAction.h"
#include "BattleEvent.h"

class SkillLianHuan : public SkillAction, public entityx::Receiver<SkillLianHuan>
{
public:
    ~SkillLianHuan();
    
    void fireAction(entityx::EntityManager &es) override;
    
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
    void receive(const BattleEvent::HurtByAttack& e);
    
private:
    void createHeroBeingHurtEffect(entityx::Entity& entity);
    void createLineEffect(const cocos2d::Vec3& fromPos, const cocos2d::Vec3& toPos, float delay);
    
    float m_radius = 0;
    float m_duration = 0;
    float m_hurtPercent = 0;
};

#endif /* defined(__sanguoGame__SkillLianHuan__) */
