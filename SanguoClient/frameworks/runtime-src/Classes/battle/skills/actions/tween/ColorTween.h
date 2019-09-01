//
//  ColorTween.h
//  sanguoClient
//
//  Created by fuchenhao on 4/10/15.
//
//

#ifndef __COLOR_TWEEN_H__
#define __COLOR_TWEEN_H__

#include "SkillAction.h"

class ColorTween : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__COLOR_TWEEN_H__) */
