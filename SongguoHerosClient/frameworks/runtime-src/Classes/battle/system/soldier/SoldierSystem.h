//
//  SoldierSystem.h
//  Game
//
//  Created by fuchenhao on 12/22/14.
//
//

#ifndef __SOLDIER_SYSTEM_H__
#define __SOLDIER_SYSTEM_H__


#include "../../../entityx/entityx.h"
#include "BattleComponent.h"
#include "BattleConfig.h"
#include "TicketRequest.h"

/*
void inline doSubState(BattleComponent::Soldier::Handle& soldier, int state)
{
    if(state == BattleConfig::MOVE_SOLDIER_ARRIVE)
    {
        soldier->subState = BattleConfig::SUB_STATE_WAIT;
    }
    else if(state == BattleConfig::MOVE_SOLDIER_BLOCK)
    {
        soldier->subState = BattleConfig::SUB_STATE_WAIT;
    }
    else //wind
    {
        soldier->subState = BattleConfig::SUB_STATE_WIND;
    }
    
}
*/

/**
 we define all state in two cetegories
 
 1: strategy state
 they are simple state come from strategy, it finish the basic behavior of the actors
 they can be switched from each other directly, and will not take any conflict
 
 2: special state
 they are come from AI, to finish special behavior, for example, MoveToTargetTile state is used to move soldier around to save room to hero
 there are always some CONDITIONS if actor switch to these states, and usally need to switch back to previous / idle states to continue the state cycle
 
 3: skill state
 */
static bool isSoldierStrategyState(int objectStateCode)
{
    return(
           objectStateCode == BattleConfig::STATE_IDLE
           
           || objectStateCode == BattleConfig::STATE_GENERAL
           || objectStateCode == BattleConfig::STATE_SOLDIER
           
           || objectStateCode == BattleConfig::STATE_BACK
           || objectStateCode == BattleConfig::STATE_CENTER
           || objectStateCode == BattleConfig::STATE_DEPART
           
           || objectStateCode == BattleConfig::STATE_PROTECT
           );
    
}


static bool isSoldierModeState(int objectStateCode)
{
    return(
           objectStateCode == BattleConfig::STATE_DIZZY
           );
    
}

class SoldierSystem : public entityx::System<SoldierSystem>
{
public:
    SoldierSystem()
    {
        m_findTargetInIdleTicketRequest.reset();
//        m_checkBowManAttackTicketRequest.reset();
    }
    
    static TicketRequest& getFindTargetInIdleTicketRequest() { return m_findTargetInIdleTicketRequest; }
//    static TicketRequest& getCheckBowManAttackTicketRequest() { return m_checkBowManAttackTicketRequest; }
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
private:
    static TicketRequest m_findTargetInIdleTicketRequest;
    
    static float s_maxPosX;
//    static TicketRequest m_checkBowManAttackTicketRequest;

};

#endif
