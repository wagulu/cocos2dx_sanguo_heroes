//
//  AddEntityAnimation.h
//  sanguoClient
//
//  Created by fuchenhao on 4/16/15.
//
//

#ifndef __sanguoClient__AddEntityAnimation__
#define __sanguoClient__AddEntityAnimation__

#include "SkillAction.h"

class AddEntityAnimation : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__AddEntityAnimation__) */
