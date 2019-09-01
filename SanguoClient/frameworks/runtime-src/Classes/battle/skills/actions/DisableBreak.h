//
//  DisableBreak.h
//  sanguoClient
//
//  Created by fuchenhao on 6/2/15.
//
//

#ifndef __sanguoClient__DisableBreak__
#define __sanguoClient__DisableBreak__

#include "SkillAction.h"
#include "BattleEvent.h"

class DisableBreak : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__DisableBreak__) */
