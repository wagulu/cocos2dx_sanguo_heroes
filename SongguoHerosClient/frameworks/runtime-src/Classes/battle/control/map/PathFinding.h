//
//  PathFinding.h
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#ifndef __PATH_FINDING_H__
#define __PATH_FINDING_H__

#include "BattleEntity.h"
#include "MapHelper.h"
#include <vector>

class PathFinding
{
private:
    struct TargetTile
    {
        int x;
        int z;
        TargetTile(int x = -0xcdcd, int z = -0xcdcd): x(x), z(z) {}
        bool valid() { return x != -0xcdcd && z != -0xcdcd; }
        void set(int x, int z) { this->x = x; this->z = z; }
    };
    
public:
    PathFinding(BattleEntity* entities);
    ~PathFinding();
    
    bool randomRun(entityx::Entity& entity);
    TargetTile pickupTargetTile(entityx::Entity& entity, bool force);
    bool roll(entityx::Entity& entity, bool force);
    enum BattleConfig::MoveSoldierResult moveDepart(entityx::Entity& entity);
    enum BattleConfig::MoveSoldierResult moveCenter(entityx::Entity& entity);
    enum BattleConfig::MoveSoldierResult moveBack(entityx::Entity& entity);
    bool arriveMiddle(BattleComponent::Position::Handle& position);
    
private:
    
    BattleEntity* m_entities;

};

#endif /* defined(__PATH_FINDING_H__) */
