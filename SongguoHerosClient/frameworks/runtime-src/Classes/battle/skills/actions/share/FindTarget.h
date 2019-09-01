//
//  FindTarget.h
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#ifndef __sanguoClient__FindTarget__
#define __sanguoClient__FindTarget__

#include "SkillAction.h"

class FindTarget : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__FindTarget__) */
