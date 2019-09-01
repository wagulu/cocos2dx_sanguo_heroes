//
//  AddEntityEffect.h
//  sanguoClient
//
//  Created by fuchenhao on 4/18/15.
//
//

#ifndef __sanguoClient__AddEntityEffect__
#define __sanguoClient__AddEntityEffect__

#include "SkillAction.h"

class AddEntityEffect : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__AddEntityEffect__) */
