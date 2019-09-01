//
//  EnemyIsClose.h
//  sanguoClient
//
//  Created by fuchenhao on 4/3/15.
//
//

#ifndef __ENEMY_IS_CLOSE__
#define __ENEMY_IS_CLOSE__

#include "AICondition.h"

class EnemyIsClose : public AICondition
{
public:
    EnemyIsClose(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};



#endif /* defined(__ENEMY_IS_CLOSE__) */
