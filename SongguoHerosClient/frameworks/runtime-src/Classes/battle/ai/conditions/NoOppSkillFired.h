//
//  NoOppSkillFired.h
//  sanguoClient
//
//  Created by fuchenhao on 6/10/15.
//
//

#ifndef __sanguoClient__NoOppSkillFired__
#define __sanguoClient__NoOppSkillFired__

#include "../AICondition.h"
#include "../../../entityx/entityx.h"
#include "BattleEvent.h"

class NoOppSkillFired : public AICondition, public entityx::Receiver<NoOppSkillFired>
{
public:
    NoOppSkillFired(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~NoOppSkillFired();
    
    void configure() override;
    void receive(const BattleEvent::CompleteSkill& e);
    
    // break or lock_user_control
};

#endif /* defined(__sanguoClient__NoOppSkillFired__) */
