//
//  OppHeroAroundSelfSoldiersLessThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__OppHeroAroundSelfSoldiersLessThan__
#define __sanguoClient__OppHeroAroundSelfSoldiersLessThan__

#include "../AICondition.h"

class OppHeroAroundSelfSoldiersLessThan : public AICondition
{
public:
    OppHeroAroundSelfSoldiersLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__sanguoClient__OppHeroAroundSelfSoldiersLessThan__) */
