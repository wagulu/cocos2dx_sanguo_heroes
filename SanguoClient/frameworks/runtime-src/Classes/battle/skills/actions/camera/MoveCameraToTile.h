//
//  MoveCameraToTile.h
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#ifndef __sanguoClient__MoveCameraToTile__
#define __sanguoClient__MoveCameraToTile__

#include "SkillAction.h"

class MoveCameraToTile : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__MoveCameraToTile__) */
