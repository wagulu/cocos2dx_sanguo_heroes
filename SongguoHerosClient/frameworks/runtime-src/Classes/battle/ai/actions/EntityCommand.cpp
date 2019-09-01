//
//  EntityCommand.cpp
//  Game
//
//  Created by fuchenhao on 3/12/15.
//
//

#include "EntityCommand.h"
#include "BattleWorld.h"


void SoldierKillSoldier::tryAction()
{
    bool isLeft = (m_side == BattleConfig::SIDE_LEFT) ? true : false;
    _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_ATTACK_SOLDIER, isLeft);
    m_completed = true;
}

void SoldierKillHero::tryAction()
{
    bool isLeft = (m_side == BattleConfig::SIDE_LEFT) ? true : false;
    _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_ATTACK_HERO, isLeft);
    m_completed = true;
}

void SoldierGoDepart::tryAction()
{
    bool isLeft = (m_side == BattleConfig::SIDE_LEFT) ? true : false;
    _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_DEAPRT, isLeft);
    m_completed = true;
}

void SoldierGoBack::tryAction()
{
    bool isLeft = (m_side == BattleConfig::SIDE_LEFT) ? true : false;
    _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_BACK, isLeft);
    m_completed = true;
}

void SoldierGoCenter::tryAction()
{
    bool isLeft = (m_side == BattleConfig::SIDE_LEFT) ? true : false;
    _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_RECENTER, isLeft);
    m_completed = true;
}

void SoldierIdle::tryAction()
{
    bool isLeft = (m_side == BattleConfig::SIDE_LEFT) ? true : false;
    _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_STOP, isLeft);
    m_completed = true;
}

void SoldierProtectHero::tryAction()
{
    bool isLeft = (m_side == BattleConfig::SIDE_LEFT) ? true : false;
    _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_PROTECT_HERO, isLeft);
    m_completed = true;
}

void HeroGo::tryAction()
{
    _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_RUSH_OUT, m_heroId);
    m_completed = true;
}

void HeroStop::tryAction()
{
    _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_STOP, m_heroId);
    m_completed = true;
}


