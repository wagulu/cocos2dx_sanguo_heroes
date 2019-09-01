//
//  OppHeroAroundSelfSoldiersMoreThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__OppHeroAroundSelfSoldiersMoreThan__
#define __sanguoClient__OppHeroAroundSelfSoldiersMoreThan__

#include "../AICondition.h"

class OppHeroAroundSelfSoldiersMoreThan : public AICondition
{
public:
    OppHeroAroundSelfSoldiersMoreThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__sanguoClient__OppHeroAroundSelfSoldiersMoreThan__) */
