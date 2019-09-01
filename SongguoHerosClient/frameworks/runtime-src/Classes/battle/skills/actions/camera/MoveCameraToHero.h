//
//  MoveCameraToHero.h
//  SG
//
//  Created by fuchenhao on 15-3-6.
//
//

#ifndef __Game__MoveCameraToHero__
#define __Game__MoveCameraToHero__

#include "SkillAction.h"

class MoveCameraToHero : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__Game__MoveCameraToHero__) */

