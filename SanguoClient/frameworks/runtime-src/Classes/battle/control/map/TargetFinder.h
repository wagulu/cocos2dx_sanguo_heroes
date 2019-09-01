//
//  TargetFinder.h
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#ifndef __TARGET_FINDER_H__
#define __TARGET_FINDER_H__

#include "BattleEntity.h"
#include "MapHelper.h"

class TargetFinder
{
public:
    
    enum TargetType
    {
        TARGET_ALL,
        TARGET_HERO,
        TARGET_SOLDIER
    };
    
    struct TargetResults
    {
        std::vector<int> targets;
        
        void add(int targetId) { targets.push_back(targetId); }
    };
    
    TargetFinder(BattleEntity* entities);
    ~TargetFinder();
    
    void findTarget(entityx::Entity& entity);
    bool findTargetNearby(entityx::Entity& entity);
    
    void findAllTarget();
    void findCenter(int side, int size, BattleConfig::Tile& center);
    
    /* 寻找英雄身前身后的目标，如果身前身后同时有目标，则随机指向身前或身后的目标 */
    int findHeroNearbyTarget(entityx::Entity& entity);
    int findHeroFrontNearbyTarget(entityx::Entity& entity);
    int findHeroBackNearbyTarget(entityx::Entity& entity);
    int findHeroBlockMoveTarget(entityx::Entity& entity);
    int findHeroTargetAtTile(entityx::Entity& entity, int tileZ, int tileX, TargetType targetType = TARGET_ALL);
    int findHeroOpponentHero(entityx::Entity& entity);
    bool findEnemyHeroOnSameLine(entityx::Entity& entity);
    
    void findDiamondAreaTilesCenterOnHero(int radius, entityx::Entity& heroEntity, BattleConfig::TileResults& results);
    void findRectAreaTilesOnHero(int width, int height, entityx::Entity& heroEntity, BattleConfig::TileResults& results, bool reverse = false);
    void findDiamondAreaTilesCenterOnTile(int radius, int tileZ, int tileX, BattleConfig::TileResults& results);
    void findRectAreaArroundHero(int width, int height, entityx::Entity& heroEntity, BattleConfig::TileResults& results);
    void findRectAreaArroundTile(int width, int height, int centerX, int centerY, BattleConfig::TileResults& results);
    void findRectAreaTilesCenterOnTile(int radius, int tileZ, int tileX, BattleConfig::TileResults& results);
    void findSkillTargets(int fromHeroId, BattleConfig::SkillTarget skillTarget, TargetResults& results);
    
    bool findBowmanTarget(entityx::Entity& entity);
    bool findBowmanTargetRightward(entityx::Entity& entity);
    bool findBowmanTargetLeftward(entityx::Entity& entity);
//    void cleanCheckTargetInLine();

private:
    
    BattleEntity* m_entities;
    
    float calcEntityWeight(entityx::Entity& entity, BattleComponent::Position::Handle position, entityx::Entity& targetEntity);
    
//    char m_checkTargetInLineCacheLeft[STAGE_HEIGHT];
//    char m_checkTargetInLineCacheRight[STAGE_HEIGHT];
//    int getCheckInLineCache(int side, int tileZ);
//    void setCheckInLineCache(int side, int tileZ, int value);
};

#endif /* defined(__TARGET_FINDER_H__) */
