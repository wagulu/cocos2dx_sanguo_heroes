//
//  SkillShanXian.h
//  sanguoClient
//
//  Created by fuchenhao on 4/28/15.
//
//

#ifndef __sanguoClient__SkillShanXian__
#define __sanguoClient__SkillShanXian__

#include "SkillAction.h"

class SkillShanXian : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__SkillShanXian__) */
