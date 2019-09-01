//
//  AttackRectArroundHero.h
//  sanguoClient
//
//  Created by fuchenhao on 5/8/15.
//
//

#ifndef __sanguoClient__AttackRectArroundHero__
#define __sanguoClient__AttackRectArroundHero__

#include "SkillAction.h"

class AttackRectArroundHero : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__AttackRectArroundHero__) */
