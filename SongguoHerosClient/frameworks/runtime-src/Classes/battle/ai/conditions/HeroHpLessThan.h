//
//  HeroHp.h
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#ifndef __HERO_HP_LESS_THAN_H__
#define __HERO_HP_LESS_THAN_H__


#include "../AICondition.h"

class HeroHpLessThan : public AICondition
{
public:
    HeroHpLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__HERO_HP_LESS_THAN_H__) */
