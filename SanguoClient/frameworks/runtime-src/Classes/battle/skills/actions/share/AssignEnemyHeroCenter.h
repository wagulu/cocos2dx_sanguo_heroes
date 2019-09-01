//
//  AssignEnemyHeroCenter.h
//  sanguoGame
//
//  Created by fuchenhao on 7/14/15.
//
//

#ifndef __sanguoGame__AssignEnemyHeroCenter__
#define __sanguoGame__AssignEnemyHeroCenter__

#include "SkillAction.h"

class AssignEnemyHeroCenter : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoGame__AssignEnemyHeroCenter__) */
