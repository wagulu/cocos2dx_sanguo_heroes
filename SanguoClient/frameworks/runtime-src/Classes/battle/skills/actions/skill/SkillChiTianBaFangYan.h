//
//  SkillChiTianBaFangYan.h
//  Game
//
//  Created by fuchenhao on 3/6/15.
//
//

#ifndef __Game__SkillChiTianBaFangYan__
#define __Game__SkillChiTianBaFangYan__

#include "SkillAction.h"

class SkillChiTianBaFangYan : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__Game__SkillChiTianBaFangYan__) */
