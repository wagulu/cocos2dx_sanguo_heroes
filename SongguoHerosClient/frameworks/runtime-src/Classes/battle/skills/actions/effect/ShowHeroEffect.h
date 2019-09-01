//
//  ShowHeroEffect.h
//  Game
//
//  Created by fuchenhao on 3/3/15.
//
//

#ifndef __SHOW_HERO_EFFECT__
#define __SHOW_HERO_EFFECT__

#include "SkillAction.h"

class ShowHeroEffect : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
private:
    void fire();
    float m_elapse = 0;
    float m_delay;
};


#endif /* defined(__Game__ShowHeroEffect__) */
