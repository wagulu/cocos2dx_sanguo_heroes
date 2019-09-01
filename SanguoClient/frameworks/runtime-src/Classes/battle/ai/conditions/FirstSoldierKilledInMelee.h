//
//  FirstSoldierKilledInMelee.h
//  Game
//
//  Created by fuchenhao on 3/27/15.
//
//

#ifndef __FIRST_SOLDIER_KILLED_IN_MELEE_H__
#define __FIRST_SOLDIER_KILLED_IN_MELEE_H__


#include "../AICondition.h"
#include "../../../entityx/entityx.h"
#include "BattleEvent.h"

class FirstSoldierKilledInMelee : public AICondition, public entityx::Receiver<FirstSoldierKilledInMelee>
{
public:
    FirstSoldierKilledInMelee(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~FirstSoldierKilledInMelee();
    
    void configure() override;
    void receive(const BattleEvent::BattleActorDead& e);
};


#endif /* defined(__FIRST_SOLDIER_KILLED_IN_MELEE_H__) */
