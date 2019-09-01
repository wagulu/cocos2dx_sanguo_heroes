//
//  BattleStartCondition.h
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#ifndef __BATTLE_START_H__
#define __BATTLE_START_H__

#include "../AICondition.h"
#include "BattleEvent.h"

class BattleStart : public AICondition
{
public:
    BattleStart(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};

#endif /* defined(__BATTLE_START_H__) */
