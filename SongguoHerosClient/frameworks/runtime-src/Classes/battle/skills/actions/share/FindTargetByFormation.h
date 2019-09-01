//
//  FindTargetByFormation.h
//  sanguoClient
//
//  Created by fuchenhao on 5/25/15.
//
//

#ifndef __sanguoClient__FindTargetByFormation__
#define __sanguoClient__FindTargetByFormation__

#include "SkillAction.h"

class FindTargetByFormation : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__FindTargetByFormation__) */
