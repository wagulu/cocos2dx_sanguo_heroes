//
//  AddBuff.h
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#ifndef __Game__AddBuff__
#define __Game__AddBuff__

#include "SkillAction.h"
#include "AddBuffBase.h"

class AddBuff : public AddBuffBase
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__Game__AddBuff__) */
