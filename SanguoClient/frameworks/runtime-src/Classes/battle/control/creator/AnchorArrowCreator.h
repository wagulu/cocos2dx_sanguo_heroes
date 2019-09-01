//
//  AnchorArrowCreator.h
//  sanguoClient
//
//  Created by fu.chenhao on 4/16/15.
//
//

#ifndef __sanguoClient__AnchorArrowCreator__
#define __sanguoClient__AnchorArrowCreator__

#include <stdio.h>
#include "BattleEntity.h"
#include "MapHelper.h"
#include "BattleEvent.h"
#include "BattleStage.h"

class AnchorArrowCreator
{
public:
    
    AnchorArrowCreator(BattleEntity* entities, BattleStage* stage);
    ~AnchorArrowCreator();
    
    bool addArrow(entityx::Entity firer, int direction);
    
private:
    
    BattleEntity* m_entities;
    BattleStage* m_stage;
};

#endif /* defined(__sanguoClient__AnchorArrowCreator__) */
