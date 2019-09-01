//
//  SelfMainHeroBeAttacked.h
//  sanguoClient
//
//  Created by fuchenhao on 4/9/15.
//
//

#ifndef __sanguoClient__SelfMainHeroBeAttacked__
#define __sanguoClient__SelfMainHeroBeAttacked__

#include "AICondition.h"
#include "entityx.h"
#include "BattleEvent.h"

class SelfMainHeroBeAttacked : public AICondition, public entityx::Receiver<SelfMainHeroBeAttacked>
{
public:
    SelfMainHeroBeAttacked(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~SelfMainHeroBeAttacked();
    
    void configure() override;
    void receive(const BattleEvent::BeAttacked& e);
};

#endif /* defined(__sanguoClient__SelfMainHeroBeAttacked__) */
