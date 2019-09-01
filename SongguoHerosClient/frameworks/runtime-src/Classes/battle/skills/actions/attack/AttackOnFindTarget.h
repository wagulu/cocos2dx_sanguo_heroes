//
//  AttackOnFindTarget.h
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#ifndef __sanguoClient__AttackOnFindTarget__
#define __sanguoClient__AttackOnFindTarget__

#include "SkillAction.h"

class AttackOnFindTarget : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__AttackOnFindTarget__) */
