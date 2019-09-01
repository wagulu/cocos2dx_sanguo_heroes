//
//  FindTileOnEnemyHero.h
//  sanguoGame
//
//  Created by fuchenhao on 8/3/15.
//
//

#ifndef __sanguoGame__FindTileOnEnemyHero__
#define __sanguoGame__FindTileOnEnemyHero__

#include "SkillAction.h"

class FindTileOnEnemyHero : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoGame__FindTileOnEnemyHero__) */
