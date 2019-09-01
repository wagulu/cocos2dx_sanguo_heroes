//
//  SetStateDizzyOnFindTarget.h
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#ifndef __sanguoClient__SetStateDizzyOnFindTarget__
#define __sanguoClient__SetStateDizzyOnFindTarget__

#include "SkillAction.h"
#include "BattleEvent.h"
#include "SetStateDizzyBase.h"

class SetStateDizzyOnFindTarget : public SetStateDizzyBase
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__SetStateDizzyOnFindTarget__) */
