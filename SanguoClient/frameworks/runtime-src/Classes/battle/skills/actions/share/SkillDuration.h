//
//  SkillDuration.h
//  sanguoClient
//
//  Created by fuchenhao on 4/16/15.
//
//

#ifndef __sanguoClient__SkillDuration__
#define __sanguoClient__SkillDuration__

#include "SkillAction.h"

class SkillDuration : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__SkillDuration__) */
