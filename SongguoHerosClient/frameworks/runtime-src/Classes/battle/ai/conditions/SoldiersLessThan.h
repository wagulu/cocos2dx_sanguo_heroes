//
//  SoldierLessThan.h
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#ifndef __SOLDIERS_LESS_THAN_H__
#define __SOLDIERS_LESS_THAN_H__


#include "../AICondition.h"

class SoldiersLessThan : public AICondition
{
public:
    SoldiersLessThan(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};

#endif /* defined(__SOLDIERS_LESS_THAN_H__) */
