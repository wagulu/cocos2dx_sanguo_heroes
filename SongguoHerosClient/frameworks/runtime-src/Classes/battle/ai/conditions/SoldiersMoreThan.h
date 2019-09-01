//
//  SoldiersMoreThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__SoldiersMoreThan__
#define __sanguoClient__SoldiersMoreThan__

#include "../AICondition.h"

class SoldiersMoreThan : public AICondition
{
public:
    SoldiersMoreThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};

#endif /* defined(__sanguoClient__SoldiersMoreThan__) */
