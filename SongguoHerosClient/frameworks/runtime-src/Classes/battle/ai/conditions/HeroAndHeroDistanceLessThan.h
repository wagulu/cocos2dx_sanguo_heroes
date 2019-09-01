//
//  HeroDistanceLessThan.h
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#ifndef __HERO_DISTANCE_LESS_THAN__
#define __HERO_DISTANCE_LESS_THAN__

#include "../AICondition.h"

class HeroAndHeroDistanceLessThan : public AICondition
{
public:
    HeroAndHeroDistanceLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__HERO_DISTANCE_LESS_THAN__) */
