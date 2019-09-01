//
//  FindRandomTargetAroundTile.h
//  sanguoGame
//
//  Created by fuchenhao on 7/14/15.
//
//

#ifndef __sanguoGame__FindRandomTargetAroundTile__
#define __sanguoGame__FindRandomTargetAroundTile__


#include "SkillAction.h"

class FindRandomTargetAroundTile : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__sanguoGame__FindRandomTargetAroundTile__) */
