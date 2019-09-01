//
//  SkillBaJiaoShan.h
//  sanguoGame
//
//  Created by fuchenhao on 7/29/15.
//
//

#ifndef __sanguoGame__SkillBaJiaoShan__
#define __sanguoGame__SkillBaJiaoShan__

#include "SkillAction.h"

class SkillBaJiaoShan : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoGame__SkillBaJiaoShan__) */
