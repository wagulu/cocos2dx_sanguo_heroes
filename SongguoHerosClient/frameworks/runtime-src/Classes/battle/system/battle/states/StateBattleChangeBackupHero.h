//
//  StateBattleChangeBackupHero.h
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#ifndef __sanguoGame__StateBattleChangeBackupHero__
#define __sanguoGame__StateBattleChangeBackupHero__

#include "IState.h"
#include "entityx.h"
#include "BattleSystem.h"
#include "BattleEvent.h"

class StateBattleChangeBackupHero : public IState, public entityx::Receiver<StateBattleChangeBackupHero>
{
public:
    enum RESULT
    {
        WORKING,
        COMPLETE,
        CREATE_BACKUP_HERO_FAIL
    };
    RESULT m_result = WORKING;
    
    virtual int getStateCode()
    {
        return BattleSystem::BattleState::CHANGE_BACKUP_HERO;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    void receive(const BattleEvent::StateHeroDeadDone& e);
    void receive(const BattleEvent::DialogueResultComplete& e);
    
private:
    bool createBackupHero(int heroId, int atTileZ);
};

#endif /* defined(__sanguoGame__StateBattleChangeBackupHero__) */
