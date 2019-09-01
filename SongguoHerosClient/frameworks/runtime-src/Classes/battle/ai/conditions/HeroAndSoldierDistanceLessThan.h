//
//  HeroAndSoldierDistanceLessThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#ifndef __sanguoClient__HeroAndSoldierDistanceLessThan__
#define __sanguoClient__HeroAndSoldierDistanceLessThan__


#include "../AICondition.h"

class HeroAndSoldierDistanceLessThan : public AICondition
{
public:
    HeroAndSoldierDistanceLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__sanguoClient__HeroAndSoldierDistanceLessThan__) */
