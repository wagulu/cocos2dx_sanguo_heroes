//
//  ShakeCamera.h
//  Game
//
//  Created by fuchenhao on 15-3-16.
//
//

#ifndef __Game__ShakeCamera__
#define __Game__ShakeCamera__

#include "SkillAction.h"

class ShakeCamera : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__Game__ShakeCamera__) */
