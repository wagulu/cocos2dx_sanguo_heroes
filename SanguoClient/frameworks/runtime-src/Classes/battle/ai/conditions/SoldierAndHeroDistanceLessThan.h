//
//  SoldierAndHeroDistanceLessThan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#ifndef __sanguoClient__SoldierAndHeroDistanceLessThan__
#define __sanguoClient__SoldierAndHeroDistanceLessThan__

#include "../AICondition.h"

class SoldierAndHeroDistanceLessThan : public AICondition
{
public:
    SoldierAndHeroDistanceLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__sanguoClient__SoldierAndHeroDistanceLessThan__) */
