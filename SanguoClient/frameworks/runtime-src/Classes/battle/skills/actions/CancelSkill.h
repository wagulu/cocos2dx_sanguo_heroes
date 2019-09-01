//
//  CancelSkill.h
//  sanguoGame
//
//  Created by fuchenhao on 7/28/15.
//
//

#ifndef __sanguoGame__CancelSkill__
#define __sanguoGame__CancelSkill__

#include "SkillAction.h"

class CancelSkill : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoGame__CancelSkill__) */
