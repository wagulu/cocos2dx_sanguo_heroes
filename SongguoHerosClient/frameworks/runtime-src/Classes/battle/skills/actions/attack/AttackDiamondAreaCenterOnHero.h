//
//  AttackDiamondAreaCenterOnHero.h
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#ifndef __Game__AttackDiamondAreaCenterOnHero__
#define __Game__AttackDiamondAreaCenterOnHero__

#include "SkillAction.h"

class AttackDiamondAreaCenterOnHero : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__Game__AttackDiamondAreaCenterOnHero__) */
