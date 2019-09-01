//
//  HeroHpPercentLessThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/8/15.
//
//

#ifndef __sanguoClient__HeroHpPercentLessThan__
#define __sanguoClient__HeroHpPercentLessThan__

#include "../AICondition.h"

class HeroHpPercentLessThan : public AICondition
{
public:
    HeroHpPercentLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};

#endif /* defined(__sanguoClient__HeroHpPercentLessThan__) */
