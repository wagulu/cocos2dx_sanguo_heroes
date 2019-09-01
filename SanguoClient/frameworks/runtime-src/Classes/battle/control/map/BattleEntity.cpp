//
//  EntityManager.cpp
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#include "BattleEntity.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "CCLuaEngine.h"

BattleEntity::BattleEntity()
{
    m_battle = _ENTITY_MANAGER.create();
    m_battle.assign<BattleComponent::Battle>(BattleConfig::getInstance()->getBattleFieldConfig().totalTime);
    m_battle.assign<BattleComponent::ObjectStateMachine>(m_battle);
    
    memset(m_map, 0, sizeof(m_map));
    
    m_leftSoldierId = LEFT_SOLDIER;
    m_rightSoldierId = RIGHT_SOLDIER;
    m_leftHeroId = LEFT_HERO;
    m_rightHeroId = RIGHT_HERO;
}

BattleEntity::~BattleEntity()
{
    for (auto* pBackupHero : m_leftBackupHero)
    {
        delete pBackupHero;
    }
    m_leftBackupHero.clear();
    
    for (auto* pBackupHero : m_rightBackupHero)
    {
        delete pBackupHero;
    }
    m_rightBackupHero.clear();
}

void BattleEntity::addEntity(entityx::Entity& entity, int tileZ, int tileX)
{
    auto identify = entity.component<BattleComponent::Identify>();
    
    assert(!m_entities[identify->id].valid());
    m_entities[identify->id] = entity;
//    m_entities.insert(std::pair<int, entityx::Entity>(identify->id, entity));
    
    if(identify->type == BattleConfig::ENTITY_SOLDIER)
    {
        if(identify->side == BattleConfig::SIDE_LEFT)
        {
            m_leftSoldiers.push_back(entity);
        }
        else if(identify->side == BattleConfig::SIDE_RIGHT)
        {
            m_rightSoldiers.push_back(entity);
        }
    }
    else if(identify->type == BattleConfig::ENTITY_HERO)
    {
        if(identify->side == BattleConfig::SIDE_LEFT)
        {
            m_leftHeros.push_back(entity);
        }
        else if(identify->side == BattleConfig::SIDE_RIGHT)
        {
            m_rightHeros.push_back(entity);
        }
        
        if (tileX - 1 >= 0)
        {
            m_map[tileZ][tileX - 1] = identify->id;
        }
    }
    
    m_map[tileZ][tileX] = identify->id;
}

void BattleEntity::removeEntity(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    
    assert(m_entities[identify->id].id() == entity.id());
    m_entities[identify->id] = entityx::Entity();
//    m_entities.erase(identify->id);
    
    if(identify->type == BattleConfig::ENTITY_SOLDIER)
    {
        if(identify->side == BattleConfig::SIDE_LEFT)
        {
            eraseEntity(m_leftSoldiers, entity);
        }
        else if(identify->side == BattleConfig::SIDE_RIGHT)
        {
            eraseEntity(m_rightSoldiers, entity);
        }
        else
        {
            assert(false);
        }
        setSoldierDead(entity);
    }
    else if(identify->type == BattleConfig::ENTITY_HERO)
    {
        if(identify->side == BattleConfig::SIDE_LEFT)
        {
            eraseEntity(m_leftHeros, entity);
        }
        else if(identify->side == BattleConfig::SIDE_RIGHT)
        {
            eraseEntity(m_rightHeros, entity);
        }
        else
        {
            assert(false);
        }
        setHeroDead(entity);
    }
}

void BattleEntity::eraseEntity(std::vector<entityx::Entity>& entities, entityx::Entity& entity)
{
    auto iter = std::find(entities.begin(), entities.end(), entity);
    entities.erase(iter);
//    for(auto it = entities.begin(); it != entities.end(); it++)
//    {
//        if(*it == entity)
//        {
//            entities.erase(it);
//            break;
//        }
//    }
}

int BattleEntity::getSoldierId(BattleConfig::Side side)
{
    if (side == BattleConfig::SIDE_LEFT)
    {
        assert(m_leftSoldierId > 0 && m_leftSoldierId < RIGHT_SOLDIER);
        return m_leftSoldierId++;
    }
    else
    {
        assert(m_rightSoldierId >= RIGHT_SOLDIER && m_rightSoldierId < LEFT_HERO);
        return m_rightSoldierId++;
    }
}

int BattleEntity::getHeroId(BattleConfig::Side side)
{
    if (side == BattleConfig::SIDE_LEFT)
    {
        assert(m_leftHeroId >= LEFT_HERO && m_leftHeroId < RIGHT_HERO);
        return m_leftHeroId++;
    }
    else
    {
        assert(m_rightHeroId >= RIGHT_HERO && m_rightHeroId < ACTOR_ID_MAX);
        return m_rightHeroId++;
    }
}

void BattleEntity::setSoldierDead(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto position = entity.component<BattleComponent::Position>();
    
    _BATTLE_STAGE.getMap()->removeChildByTag(identify->id);
    
    int tileZ = MapHelper::getTileZ(position->z);
    int tileX = MapHelper::getTileX(position->x);
    setTarget(tileZ, tileX, 0, identify->id);
}

void BattleEntity::setHeroDead(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto position = entity.component<BattleComponent::Position>();
    
    _BATTLE_STAGE.getMap()->removeChildByTag(identify->id);
    
    int tileZ = MapHelper::getTileZ(position->z);
    int tileX = MapHelper::getTileX(position->x);
    setTarget(tileZ, tileX, 0, identify->id);
    if (tileX > 0)
    {
        setTarget(tileZ, tileX - 1, 0, identify->id);
    }
}

entityx::Entity* BattleEntity::getEntity(int id)
{
    entityx::Entity& entity = m_entities[id];
    if(/*!entity.valid() ||*/ isDead(entity)) return NULL;
    return &entity;
}

//be careful when using this method, only AttackSystem should use this method to attack dead target
entityx::Entity* BattleEntity::getEntityEvenIfDead(int id)
{
    entityx::Entity& entity = m_entities[id];
    if(!entity.valid()) return nullptr;
    return &entity;
}

void BattleEntity::addBackupHero(BackupHero* pBackupHero, BattleConfig::Side side)
{
    if (side == BattleConfig::SIDE_LEFT)
    {
        m_leftBackupHero.push_back(pBackupHero);
    }
    else
    {
        m_rightBackupHero.push_back(pBackupHero);
    }
}

bool BattleEntity::hasBackupHero(BattleConfig::Side side)
{
    if (side == BattleConfig::SIDE_LEFT)
    {
        return m_leftBackupHero.size() > m_leftBackupHeroIndex;
    }
    else
    {
        return m_rightBackupHero.size() > m_rightBackupHeroIndex;
    }
    return false;
}

BattleEntity::BackupHero* BattleEntity::popNextBackupHero(BattleConfig::Side side)
{
    BattleEntity::BackupHero* pBackupHero = nullptr;
    if (hasBackupHero(side))
    {
        if (side == BattleConfig::SIDE_LEFT)
        {
            pBackupHero = m_leftBackupHero[m_leftBackupHeroIndex++];
        }
        else
        {
            pBackupHero = m_rightBackupHero[m_rightBackupHeroIndex++];
        }
    }
    return pBackupHero;
}
