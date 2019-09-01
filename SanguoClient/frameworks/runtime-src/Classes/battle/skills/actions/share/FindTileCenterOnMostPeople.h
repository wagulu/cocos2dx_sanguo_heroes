//
//  FindTileCenterOnMostPeople.h
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#ifndef __sanguoClient__FindTileCenterOnMostPeople__
#define __sanguoClient__FindTileCenterOnMostPeople__

#include "SkillAction.h"

class FindTileCenterOnMostPeople : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__sanguoClient__FindTileCenterOnMostPeople__) */
