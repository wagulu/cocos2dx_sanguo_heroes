//
//  SkillRelease.h
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#ifndef __SKILL_RELEASE_H__
#define __SKILL_RELEASE_H__


#include "../AICondition.h"
#include "../../../entityx/entityx.h"
#include "BattleEvent.h"

class SkillRelease : public AICondition, public entityx::Receiver<SkillRelease>
{
public:
    SkillRelease(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    ~SkillRelease();
    void configure() override;
    void receive(const BattleEvent::FireSkill& e);
};

#endif /* defined(__SKILL_RELEASE_H__) */
