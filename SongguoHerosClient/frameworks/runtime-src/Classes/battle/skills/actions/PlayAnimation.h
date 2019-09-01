//
//  PlayAnimation.h
//  sanguoClient
//
//  Created by fuchenhao on 5/12/15.
//
//

#ifndef __sanguoClient__PlayAnimation__
#define __sanguoClient__PlayAnimation__

#include "SkillAction.h"

class PlayAnimation : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__PlayAnimation__) */
