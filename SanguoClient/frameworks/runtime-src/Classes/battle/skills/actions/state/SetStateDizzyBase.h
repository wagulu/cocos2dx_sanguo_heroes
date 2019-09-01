//
//  SetStateDizzyBase.h
//  sanguoClient
//
//  Created by fuchenhao on 4/23/15.
//
//

#ifndef __sanguoClient__SetStateDizzyBase__
#define __sanguoClient__SetStateDizzyBase__

#include "../../SkillAction.h"

class SetStateDizzyBase : public SkillAction
{
protected:
    void setTargetDizzy(int targetId);
};

#endif /* defined(__sanguoClient__SetStateDizzyBase__) */
