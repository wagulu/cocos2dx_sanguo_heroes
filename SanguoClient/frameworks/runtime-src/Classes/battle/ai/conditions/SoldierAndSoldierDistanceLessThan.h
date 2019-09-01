//
//  SoldierAndSoldierDistanceLessThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#ifndef __sanguoClient__SoldierAndSoldierDistanceLessThan__
#define __sanguoClient__SoldierAndSoldierDistanceLessThan__

#include "../AICondition.h"

class SoldierAndSoldierDistanceLessThan : public AICondition
{
public:
    SoldierAndSoldierDistanceLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};

#endif /* defined(__sanguoClient__SoldierAndSoldierDistanceLessThan__) */
