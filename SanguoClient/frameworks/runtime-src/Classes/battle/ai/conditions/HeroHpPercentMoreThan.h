//
//  HeroHpPercentMoreThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__HeroHpPercentMoreThan__
#define __sanguoClient__HeroHpPercentMoreThan__

#include "../AICondition.h"

class HeroHpPercentMoreThan : public AICondition
{
public:
    HeroHpPercentMoreThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__sanguoClient__HeroHpPercentMoreThan__) */
