//
//  MapHelper.h
//  Game
//
//  Created by fu.chenhao on 3/3/15.
//
//

#ifndef __MAP_HELPER_H__
#define __MAP_HELPER_H__

#include "../../BattleConfig.h"

class MapHelper
{
public:
    
    static inline BattleConfig::Side getSide(int id)
    {
        return (id >= LEFT_SOLDIER && id < RIGHT_SOLDIER) || (id >= LEFT_HERO && id < RIGHT_HERO) ? BattleConfig::SIDE_LEFT : BattleConfig::SIDE_RIGHT;
    };
    
    static inline BattleConfig::EntityType getEntityType(int id)
    {
        return (id >= LEFT_SOLDIER && id < LEFT_HERO) ? BattleConfig::ENTITY_SOLDIER : BattleConfig::ENTITY_HERO;
    };
    
    static inline int getTileX(float x)
    {
        return x / GRID_WIDTH;
    }
    
    static inline int getTileZ(float z)
    {
        return z / GRID_HEIGHT;
    }
    
    static inline int getCenterX(int tileX, bool isHero)
    {
        return tileX * GRID_WIDTH + (isHero ? 0 : HALF_GRID_WIDTH);
    }
    
    static inline int getCenterZ(int tileZ)
    {
        return tileZ * GRID_HEIGHT + HALF_GRID_HEIGHT;
    }
    
    static inline bool outOfBounds(float x, float z)
    {
        return x < GRID_WIDTH || x > (STAGE_WIDTH - 3) * GRID_WIDTH - GRID_WIDTH || z < HALF_GRID_HEIGHT || z > STAGE_HEIGHT * GRID_HEIGHT - HALF_GRID_HEIGHT;
    }
    
    static inline bool validTileX(int tileX)
    {
        return tileX >= 0 && tileX < STAGE_WIDTH;
    }
    
    static inline bool validTileZ(int tileZ)
    {
        return tileZ >= 0 && tileZ < STAGE_HEIGHT;
    }
    
    static inline bool validTile(int tileZ, int tileX)
    {
        return validTileZ(tileZ) && validTileX(tileX);
    }
    
    static inline bool isHero(int id)
    {
        return getEntityType(id) == BattleConfig::ENTITY_HERO;
    }

    static inline bool isSoldier(int id)
    {
        return getEntityType(id) == BattleConfig::ENTITY_SOLDIER;
    }
    
    static inline bool isMainHero(int id)
    {
        return id == LEFT_HERO || id == RIGHT_HERO;
    }
    
};

#endif /* defined(__MAP_HELPER_H__) */
