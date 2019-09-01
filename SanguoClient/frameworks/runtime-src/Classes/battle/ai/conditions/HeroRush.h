//
//  HeroGo.h
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#ifndef __HERO_GO_H__
#define __HERO_GO_H__

#include "../AICondition.h"
#include "../../../entityx/entityx.h"
#include "BattleEvent.h"

class HeroRush : public AICondition, public entityx::Receiver<HeroRush>
{
public:
    HeroRush(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~HeroRush();
    void configure() override;
    void receive(const BattleEvent::StrategyHeroRush& e);
};

#endif /* defined(__HERO_GO_H__) */
