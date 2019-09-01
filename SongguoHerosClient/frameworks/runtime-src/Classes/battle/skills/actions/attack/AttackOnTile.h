//
//  AttackOnTile.h
//  SGGame
//
//  Created by fuchenhao on 4/27/15.
//
//

#ifndef __Game__AttackOnTile__
#define __Game__AttackOnTile__

#include "SkillAction.h"
#include "BattleEvent.h"

class AttackOnTile : public SkillAction, public entityx::Receiver<AttackOnTile>
{
public:
    ~AttackOnTile();
    
    void receive(const BattleEvent::SkillAttackOnTileEvent& e);
    
    void fireAction(entityx::EntityManager &es) override;
};

#endif /* defined(__Game__AttackOnTile__) */
