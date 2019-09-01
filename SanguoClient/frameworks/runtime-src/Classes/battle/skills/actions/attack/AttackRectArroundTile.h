//
//  AttackRectArroundTile.h
//  sanguoClient
//
//  Created by fuchenhao on 6/25/15.
//
//

#ifndef __sanguoClient__AttackRectArroundTile__
#define __sanguoClient__AttackRectArroundTile__

#include "SkillAction.h"

class AttackRectArroundTile : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__sanguoClient__AttackRectArroundTile__) */
