//
//  EntityCommand.h
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#ifndef __ENTITY_COMMAND_H__
#define __ENTITY_COMMAND_H__

#include "../AIAction.h"
#include "../../../entityx/entityx.h"

class SoldierKillSoldier : public AIAction, public entityx::Receiver<SoldierKillSoldier>
{
public:
    SoldierKillSoldier(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class SoldierKillHero : public AIAction, public entityx::Receiver<SoldierKillHero>
{
public:
    SoldierKillHero(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class SoldierGoDepart : public AIAction, public entityx::Receiver<SoldierGoDepart>
{
public:
    SoldierGoDepart(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class SoldierGoBack : public AIAction, public entityx::Receiver<SoldierGoBack>
{
public:
    SoldierGoBack(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class SoldierGoCenter : public AIAction, public entityx::Receiver<SoldierGoCenter>
{
public:
    SoldierGoCenter(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class SoldierIdle : public AIAction, public entityx::Receiver<SoldierIdle>
{
public:
    SoldierIdle(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class SoldierProtectHero : public AIAction, public entityx::Receiver<SoldierProtectHero>
{
public:
    SoldierProtectHero(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class HeroGo : public AIAction, public entityx::Receiver<HeroGo>
{
public:
    HeroGo(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

class HeroStop : public AIAction, public entityx::Receiver<HeroStop>
{
public:
    HeroStop(AIConfig::AIActionData* data, int heroId) : AIAction(data, heroId) {};
    void tryAction() override;
};

#endif /* defined(__ENTITY_COMMAND_H__) */
