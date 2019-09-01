//
//  StateBattleStartFight.h
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#ifndef __sanguoGame__StateBattleStartFight__
#define __sanguoGame__StateBattleStartFight__

#include "IState.h"
#include "entityx.h"
#include "BattleSystem.h"
#include "BattleEvent.h"

class StateBattleStartFight : public IState, public entityx::Receiver<StateBattleStartFight>
{
public:
    enum RESULT
    {
        WORKING,
        TIME_UP
    };
    RESULT m_result = WORKING;
    
    enum class SubState
    {
        SHOW_BATTLE_SCENE_UI,
        FIGHTING
    };
    
    virtual int getStateCode()
    {
        return BattleSystem::BattleState::START_FIGHT;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    void receive(const BattleEvent::ShowBattleSceneUIComplete& e);
    
private:
    SubState m_subState = SubState::SHOW_BATTLE_SCENE_UI;
};

#endif /* defined(__sanguoGame__StateBattleStartFight__) */
