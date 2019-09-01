//
//  BuffEffect.h
//  sanguoGame
//
//  Created by fuchenhao on 8/7/15.
//
//

#ifndef __sanguoGame__BuffEffect__
#define __sanguoGame__BuffEffect__

#include "SkillAction.h"

class BuffEffect : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__sanguoGame__BuffEffect__) */
