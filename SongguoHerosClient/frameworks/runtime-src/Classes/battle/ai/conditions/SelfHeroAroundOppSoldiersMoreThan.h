//
//  SoldiersMoreThan.h
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#ifndef __SOLDIERS_MORE_THAN_H__
#define __SOLDIERS_MORE_THAN_H__

#include "../AICondition.h"

class SelfHeroAroundOppSoldiersMoreThan : public AICondition
{
public:
    SelfHeroAroundOppSoldiersMoreThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};


#endif /* defined(__SOLDIERS_MORE_THAN_H__) */
