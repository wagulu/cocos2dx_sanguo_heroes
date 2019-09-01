//
//  SkillOk.h
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#ifndef __SKILL_OK_H__
#define __SKILL_OK_H__


#include "../AICondition.h"

class SkillOk : public AICondition
{
public:
    SkillOk(AIConfig::AIConditionData* data, int heroId) : AICondition(data, heroId) {};
    bool isOk() override;
};

#endif /* defined(__SKILL_OK_H__) */
