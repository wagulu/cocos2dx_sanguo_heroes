//
//  AddBuffBase.h
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#ifndef __sanguoClient__AddBuffBase__
#define __sanguoClient__AddBuffBase__

#include "SkillAction.h"

class AddBuffBase : public SkillAction
{
protected:

    void addBuffToTarget(int targetId);
};

#endif /* defined(__sanguoClient__AddBuffBase__) */
