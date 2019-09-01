//
//  SkillShunXiQianLi.h
//  SGGame
//
//  Created by fuchenhao on 4/28/15.
//
//

#ifndef __Game__SkillShunXiQianLi__
#define __Game__SkillShunXiQianLi__

#include "SkillAction.h"

class SkillShunXiQianLi : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__Game__SkillShunXiQianLi__) */
