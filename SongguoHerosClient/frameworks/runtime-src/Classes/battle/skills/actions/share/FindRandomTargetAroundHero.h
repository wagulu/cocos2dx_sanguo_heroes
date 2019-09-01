//
//  FindRandomTargetAroundHero.h
//  sanguoGame
//
//  Created by fuchenhao on 7/13/15.
//
//

#ifndef __sanguoGame__FindRandomTargetAroundHero__
#define __sanguoGame__FindRandomTargetAroundHero__


#include "SkillAction.h"

class FindRandomTargetAroundHero : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__sanguoGame__FindRandomTargetAroundHero__) */
