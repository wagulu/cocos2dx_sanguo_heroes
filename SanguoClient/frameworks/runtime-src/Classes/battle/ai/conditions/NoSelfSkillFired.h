//
//  NoSelfSkillFired.h
//  sanguoClient
//
//  Created by fuchenhao on 6/19/15.
//
//

#ifndef __sanguoClient__NoSelfSkillFired__
#define __sanguoClient__NoSelfSkillFired__

#include "../AICondition.h"
#include "../../../entityx/entityx.h"
#include "BattleEvent.h"

class NoSelfSkillFired : public AICondition, public entityx::Receiver<NoSelfSkillFired>
{
public:
    NoSelfSkillFired(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~NoSelfSkillFired();
    
    void configure() override;
    void receive(const BattleEvent::CompleteSkill& e);
    
    // break or lock_user_control
};


#endif /* defined(__sanguoClient__NoSelfSkillFired__) */
