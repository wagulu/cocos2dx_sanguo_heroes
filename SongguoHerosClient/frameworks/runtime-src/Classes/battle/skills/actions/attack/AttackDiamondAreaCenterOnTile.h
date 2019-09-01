//
//  AttackDiamondAreaCenterOnTile.h
//  sanguoClient
//
//  Created by fuchenhao on 4/10/15.
//
//

#ifndef __sanguoClient__AttackDiamondAreaCenterOnTile__
#define __sanguoClient__AttackDiamondAreaCenterOnTile__

#include "SkillAction.h"

class AttackDiamondAreaCenterOnTile : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};


#endif /* defined(__sanguoClient__AttackDiamondAreaCenterOnTile__) */
