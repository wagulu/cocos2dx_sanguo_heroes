//
//  SkillTiaoJi.h
//  sanguoGame
//
//  Created by fuchenhao on 7/16/15.
//
//

#ifndef __sanguoGame__SkillTiaoJi__
#define __sanguoGame__SkillTiaoJi__

#include "SkillAction.h"

class SkillTiaoJi : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoGame__SkillTiaoJi__) */
