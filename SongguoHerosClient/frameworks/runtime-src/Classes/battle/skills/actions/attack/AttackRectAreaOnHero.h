//
//  AttackRectAreaOnHero.h
//  sanguoClient
//
//  Created by fuchenhao on 4/14/15.
//
//

#ifndef __sanguoClient__AttackRectAreaOnHero__
#define __sanguoClient__AttackRectAreaOnHero__

#include "SkillAction.h"

class AttackRectAreaOnHero : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__sanguoClient__AttackRectAreaOnHero__) */
