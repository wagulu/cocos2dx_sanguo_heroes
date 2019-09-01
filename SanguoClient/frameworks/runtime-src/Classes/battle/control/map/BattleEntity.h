//
//  EntityManager.h
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#ifndef __BATTLE_ENTITY_H__
#define __BATTLE_ENTITY_H__

#include <map>
#include <vector>
#include "entityx.h"
#include "BattleComponent.h"
#include "BattleConfig.h"

class BattleEntity
{
public://15601088057
    
    struct BackupHero
    {
        int tileZ;
        int tileX;
        BattleConfig::HeroConfig& config;
        entityx::Entity effectEntity;
        
        BackupHero(int tileZ, int tileX, BattleConfig::HeroConfig& config, entityx::Entity& effectEntity) : tileZ(tileZ), tileX(tileX), config(config), effectEntity(effectEntity) {}
    };
    
    BattleEntity();
    ~BattleEntity();
    
    inline entityx::Entity& getBattle()
    {
        return m_battle;
    };
    
    void addEntity(entityx::Entity& entity, int tileZ, int tileX);
    void removeEntity(entityx::Entity& entity);
    
    int getSoldierId(BattleConfig::Side side);
    int getHeroId(BattleConfig::Side side);
    
    inline int getTarget(int tileZ, int tileX)
    {
        assert(tileX >= 0 && tileX < STAGE_WIDTH);
        assert(tileZ >= 0 && tileZ < STAGE_HEIGHT);
        return m_map[tileZ][tileX];
    };
    inline void setTarget(int tileZ, int tileX, int newValue, int oldValue)
    {
        assert(tileX >= 0 && tileX < STAGE_WIDTH);
        assert(tileZ >= 0 && tileZ < STAGE_HEIGHT);
        if (m_map[tileZ][tileX] == oldValue) m_map[tileZ][tileX] = newValue;
    };
    inline void changeSoldierTarget(int oldTileZ, int oldTileX, int tileZ, int tileX, int expectValue)
    {
        assert(tileX >= 0 && tileX < STAGE_WIDTH);
        assert(tileZ >= 0 && tileZ < STAGE_HEIGHT);
        assert(oldTileX >= 0 && oldTileX < STAGE_WIDTH);
        assert(oldTileZ >= 0 && oldTileZ < STAGE_HEIGHT);
        
#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
        auto entity = getEntity(expectValue);
        if(entity)
        {
            assert(m_map[oldTileZ][oldTileX] == expectValue || entity->component<BattleComponent::AI>()->isTileLosted);
            assert(m_map[tileZ][tileX] == 0 || entity->component<BattleComponent::AI>()->isTileLosted);
        }
#endif

        if (m_map[oldTileZ][oldTileX] == expectValue) m_map[oldTileZ][oldTileX] = 0;
        if (m_map[tileZ][tileX] == 0) m_map[tileZ][tileX] = expectValue;
    };
    inline void changeHeroTarget(int oldTileZ, int oldTileX, int tileZ, int tileX, int expectValue)
    {
        assert(tileX >= 1 && tileX < STAGE_WIDTH);
        assert(tileZ >= 0 && tileZ < STAGE_HEIGHT);
        assert(oldTileX >= 1 && oldTileX < STAGE_WIDTH);
        assert(oldTileZ >= 0 && oldTileZ < STAGE_HEIGHT);
        
        if (m_map[oldTileZ][oldTileX] == expectValue) m_map[oldTileZ][oldTileX] = 0;
        if (m_map[oldTileZ][oldTileX - 1] == expectValue) m_map[oldTileZ][oldTileX - 1] = 0;
        m_map[tileZ][tileX] = expectValue;
        m_map[tileZ][tileX - 1] = expectValue;
    };
    inline bool isEmpty(int tileZ, int tileX)
    {
        assert(tileX >= 0 && tileX < STAGE_WIDTH);
        assert(tileZ >= 0 && tileZ < STAGE_HEIGHT);
        return m_map[tileZ][tileX] == 0;
    };
    
    inline std::vector<entityx::Entity>& getLeftSoldiers() { return m_leftSoldiers; };
    inline std::vector<entityx::Entity>& getRightSoldiers() { return m_rightSoldiers; };
    inline std::vector<entityx::Entity>& getLeftHeros() { return m_leftHeros; };
    inline std::vector<entityx::Entity>& getRightHeros() { return m_rightHeros; };
    
    entityx::Entity* getEntity(int id);
    entityx::Entity* getEntityEvenIfDead(int id);//be careful when using this method, only AttackSystem should use this method to attack dead target
    
    void addBackupHero(BackupHero* pBackupHero, BattleConfig::Side side);
    bool hasBackupHero(BattleConfig::Side side);
    BackupHero* popNextBackupHero(BattleConfig::Side side);

private:
    void eraseEntity(std::vector<entityx::Entity>& entities, entityx::Entity& entity);
    
    void setSoldierDead(entityx::Entity& entity);
    void setHeroDead(entityx::Entity& entity);
    
    entityx::Entity m_battle;
    
    int m_map[STAGE_HEIGHT][STAGE_WIDTH];
    entityx::Entity m_entities[ACTOR_ID_MAX];
//    std::map<int, entityx::Entity> m_entities;
    std::vector<entityx::Entity> m_leftSoldiers;
    std::vector<entityx::Entity> m_rightSoldiers;
    std::vector<entityx::Entity> m_leftHeros;
    std::vector<entityx::Entity> m_rightHeros;
    
    std::vector<BackupHero*> m_leftBackupHero;
    std::vector<BackupHero*> m_rightBackupHero;
    int m_leftBackupHeroIndex = 0;
    int m_rightBackupHeroIndex = 0;
    
    int m_leftSoldierId;
    int m_rightSoldierId;
    int m_leftHeroId;
    int m_rightHeroId;
};

#endif /* defined(__BATTLE_ENTITY_H__) */
