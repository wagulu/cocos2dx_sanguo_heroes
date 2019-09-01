//
//  StateBattleChangeBackupHero.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#include "StateBattleChangeBackupHero.h"
#include "BattleWorld.h"

void StateBattleChangeBackupHero::enter(entityx::Entity& entity)
{
    _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_CONTROL_TYPE::HERO_STOP, LEFT_HERO);
    _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_CONTROL_TYPE::HERO_STOP, RIGHT_HERO);
    
    _ENTITY_EVENT.subscribe<BattleEvent::StateHeroDeadDone>(*this);
}

void StateBattleChangeBackupHero::exit(entityx::Entity& entity)
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::StateHeroDeadDone>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::StateHeroDeadDone>(*this);
    }
}

int StateBattleChangeBackupHero::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    bool allHeroInPosition = true;
    bool allHeroAlive = true;
    
    int tileX;
    BattleComponent::General::Handle hero;
    BattleComponent::Position::Handle position;
    BattleComponent::ObjectStateMachine::Handle objectStateMachine;
    
    for (entityx::Entity heroEntity : _ENTITY_MANAGER.entities_with_components(hero, position, objectStateMachine))
    {
        tileX = MapHelper::getTileX(position->x);
        if (tileX < STAGE_DUEL_LEFT_X - 2
            || tileX > STAGE_DUEL_RIGHT_X + 2
            || objectStateMachine->getCurrentStateCode() == BattleConfig::GeneralState::GENERAL_MOVE_TO_TARGET_TILE)
        {
            allHeroInPosition = false;
            break;
        }
        
        if (objectStateMachine->isDead)
        {
            allHeroAlive = false;
            break;
        }
    }
    
    if (allHeroInPosition && allHeroAlive)
    {
        _RETURN_STATE_RESULT(COMPLETE);
    }
    
    return m_result;
}

void StateBattleChangeBackupHero::receive(const BattleEvent::StateHeroDeadDone& e)
{
    auto entity = e.entity;
    auto deadHeroId = entity.component<BattleComponent::Identify>()->id;
    auto position = entity.component<BattleComponent::Position>();
    
    bool createSuccess = createBackupHero(deadHeroId, MapHelper::getTileZ(position->z));
    if (!createSuccess)
    {
        m_result = CREATE_BACKUP_HERO_FAIL;
    }
}

bool StateBattleChangeBackupHero::createBackupHero(int heroId, int atTileZ)
{
    bool createSuccess = false;
    
    auto side = MapHelper::getSide(heroId);
    auto* pBackupHero = _BATTLE_ENTITY.popNextBackupHero(side);
    if (pBackupHero != nullptr)
    {
        if (pBackupHero->effectEntity.valid())
        {
            pBackupHero->effectEntity.destroy();
        }
        
        bool addHeroSuccess = _HERO_CREATOR.addHero(heroId, pBackupHero->config, pBackupHero->tileZ, pBackupHero->tileX);
        if (addHeroSuccess)
        {
            auto* pBackupEntity = _BATTLE_ENTITY.getEntity(heroId);
            if (pBackupEntity != nullptr)
            {
                _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_CONTROL_TYPE::HERO_RUSH_OUT, heroId);
                
                BattleConfig::Tile moveToTile(atTileZ, (side == BattleConfig::SIDE_LEFT ? STAGE_DUEL_LEFT_X - 2 : STAGE_DUEL_RIGHT_X + 2));
                _ENTITY_EVENT.emit<BattleEvent::StrategyHeroMoveToTargetTile>(*pBackupEntity, moveToTile);
            }
            
            createSuccess = true;
        }
    }
    
    return createSuccess;
}
