//
//  MultiAttack.h
//  Game
//
//  Created by zhaikaiyu on 3/5/15.
//
//

#ifndef __MULTI_ATTACK_H__
#define __MULTI_ATTACK_H__

#include "SkillAction.h"

enum MultiAttackType
{
    MULTI_ATTACK_NONE,
    MULTI_ATTACK_FRONT_ENEMY,
    MULTI_ATTACK_OPP_HERO,
    MULTI_ATTACK_SELF_HERO
};

class MultiAttack : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};
#endif /* defined(__MULTI_ATTACK_H__) */
