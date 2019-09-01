//
//  TriggerCameraActions.h
//  sanguoGame
//
//  Created by fuchenhao on 8/6/15.
//
//

#ifndef __sanguoGame__TriggerCameraActions__
#define __sanguoGame__TriggerCameraActions__

#include "SkillAction.h"

class TriggerCameraActions : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoGame__TriggerCameraActions__) */
