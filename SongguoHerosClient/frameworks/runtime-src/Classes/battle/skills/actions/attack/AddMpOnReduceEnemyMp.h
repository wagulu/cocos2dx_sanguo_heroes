//
//  AddMpOnReduceEnemyMp.h
//  sanguoClient
//
//  Created by fuchenhao on 4/21/15.
//
//

#ifndef __sanguoClient__AddMpOnReduceEnemyMp__
#define __sanguoClient__AddMpOnReduceEnemyMp__

#include "SkillAction.h"

class AddMpOnReduceEnemyMp : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__AddMpOnReduceEnemyMp__) */
