//
//  ReduceEnemyMp.h
//  sanguoClient
//
//  Created by fuchenhao on 4/21/15.
//
//

#ifndef __sanguoClient__ReduceEnemyMp__
#define __sanguoClient__ReduceEnemyMp__

#include "SkillAction.h"

class ReduceEnemyMp : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__ReduceEnemyMp__) */
