//
//  AfterAnySkillRelease.h
//  sanguoClient
//
//  Created by fuchenhao on 6/8/15.
//
//

#ifndef __sanguoClient__AfterAnySkillRelease__
#define __sanguoClient__AfterAnySkillRelease__

#include "../AICondition.h"
#include "../../../entityx/entityx.h"
#include "BattleEvent.h"

class AfterAnySkillRelease : public AICondition, public entityx::Receiver<AfterAnySkillRelease>
{
public:
    AfterAnySkillRelease(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~AfterAnySkillRelease();
    
    void configure() override;
    void receive(const BattleEvent::FireSkill& e);
    bool isOk() override;
private:
    float m_elapse = 0;
    bool m_fired = false;
};



#endif /* defined(__sanguoClient__AfterAnySkillRelease__) */
