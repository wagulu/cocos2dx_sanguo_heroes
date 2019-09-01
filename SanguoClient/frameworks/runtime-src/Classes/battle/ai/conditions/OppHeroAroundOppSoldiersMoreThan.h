//
//  OppSoldiersAroundOppSoldiersMoreThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__OppSoldiersAroundOppSoldiersMoreThan__
#define __sanguoClient__OppSoldiersAroundOppSoldiersMoreThan__

#include "../AICondition.h"

class OppHeroAroundOppSoldiersMoreThan : public AICondition
{
public:
    OppHeroAroundOppSoldiersMoreThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__sanguoClient__OppSoldiersAroundOppSoldiersMoreThan__) */
