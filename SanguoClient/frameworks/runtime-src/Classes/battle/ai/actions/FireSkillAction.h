//
//  FireSkillAction.h
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#ifndef __FIRE_SKILL_ACTION_H__
#define __FIRE_SKILL_ACTION_H__

#include "../AIAction.h"
#include "../../BattleEvent.h"
#include "../../../entityx/entityx.h"

class FireSkillAction : public AIAction, public entityx::Receiver<FireSkillAction>
{
public:
    FireSkillAction(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    ~FireSkillAction();
    void configure() override;
    void tryAction() override;
    void receive(const BattleEvent::FireSkill& e);
};

#endif /* defined(__FIRE_SKILL_ACTION_H__) */
