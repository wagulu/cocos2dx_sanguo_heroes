//
//  RemoveEntityAnimation.h
//  sanguoClient
//
//  Created by fuchenhao on 4/22/15.
//
//

#ifndef __sanguoClient__RemoveEntityAnimation__
#define __sanguoClient__RemoveEntityAnimation__

#include "SkillAction.h"

class RemoveEntityAnimation : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__RemoveEntityAnimation__) */
