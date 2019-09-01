//
//  NoHeroRush.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__NoHeroRush__
#define __sanguoClient__NoHeroRush__

#include "../AICondition.h"
#include "../../../entityx/entityx.h"
#include "BattleEvent.h"

class NoHeroRush : public AICondition, public entityx::Receiver<NoHeroRush>
{
public:
    NoHeroRush(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~NoHeroRush();
    
    void configure() override;
    void receive(const BattleEvent::StrategyHeroRush& e);
};

#endif /* defined(__sanguoClient__NoHeroRush__) */
