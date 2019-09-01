//
//  OppSoldiersMoreThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#ifndef __sanguoClient__OppSoldiersMoreThan__
#define __sanguoClient__OppSoldiersMoreThan__

#include "../AICondition.h"

class OppSoldiersMoreThan : public AICondition
{
public:
    OppSoldiersMoreThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};

#endif /* defined(__sanguoClient__OppSoldiersMoreThan__) */
