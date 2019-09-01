//
//  TargetFinder.cpp
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#include "TargetFinder.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

using namespace std;

#define CHECK_TARGET_INLINE_NO_CACHE    (-128)

TargetFinder::TargetFinder(BattleEntity* entities)
{
    m_entities = entities;
    
//    cleanCheckTargetInLine();
}

TargetFinder::~TargetFinder()
{
    
}

float TargetFinder::calcEntityWeight(entityx::Entity& entity, BattleComponent::Position::Handle position, entityx::Entity& targetEntity)
{
    static const float WEIGHT_SCALE_Z = 2;

//    BattleComponent::Position::Handle position = entity.component<BattleComponent::Position>();
    BattleComponent::Position::Handle targetPosition = targetEntity.component<BattleComponent::Position>();

    float dx = targetPosition->x - position->x;
    float dz = targetPosition->z - position->z;
    float dis = fabsf(dx) + fabsf(dz) * WEIGHT_SCALE_Z;
    return dis;
    
}

void TargetFinder::findTarget(entityx::Entity& entity)
{
    if(findTargetNearby(entity))
    {
        return;
    }
    
    auto position = entity.component<BattleComponent::Position>();
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto identify = entity.component<BattleComponent::Identify>();
//    BattleComponent::Soldier::Handle targetSoldier;
    
//    int tileZ = MapHelper::getTileZ(position->z);
//    int tileX = MapHelper::getTileZ(position->x);
//    
//    int direction = (soldier->side == BattleConfig::SIDE_LEFT) ? 1 : -1;
    
    bool leftSide = false;
    int bestTargetId = -1;
    float bestWeight = INT32_MAX;

    if(identify->side == BattleConfig::SIDE_LEFT)// && m_entities->getRightSoldiers().size() == 0)
    {
        leftSide = true;
        auto* pEntity = _BATTLE_ENTITY.getEntity(RIGHT_HERO);
        if(pEntity)
        {
            bestTargetId = RIGHT_HERO;
            bestWeight = calcEntityWeight(entity, position, *pEntity);
        }
//        soldier->target = RIGHT_HERO;
//        return;
    }
    
    if(identify->side == BattleConfig::SIDE_RIGHT)// && m_entities->getLeftSoldiers().size() == 0)
    {
        leftSide = false;
        auto* pEntity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
        if(pEntity)
        {
            bestTargetId = LEFT_HERO;
            bestWeight = calcEntityWeight(entity, position, *pEntity);
        }
//        soldier->target = LEFT_HERO;
//        return;
    }
    std::vector<entityx::Entity>& targets = leftSide? _BATTLE_ENTITY.getRightSoldiers(): _BATTLE_ENTITY.getLeftSoldiers();
    
    for(entityx::Entity& target : targets)
    {
//        if(target.component<BattleComponent::Identify>()->side == entity.component<BattleComponent::Identify>()->side)
//        {
//            continue;
//        }
        
        float weight = calcEntityWeight(entity, position, target);
        if(weight < bestWeight)
        {
            bestWeight = weight;
            bestTargetId = target.component<BattleComponent::Identify>()->id;
        }
    }
    
    if(bestTargetId >= 0)
    {
        soldier->target = bestTargetId;
    }
}

bool TargetFinder::findTargetNearby(entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    int tileZ = MapHelper::getTileZ(position->z);
    int tileX = MapHelper::getTileX(position->x);
    
    int direction = (identify->side == BattleConfig::SIDE_LEFT) ? 1 : -1;
    
//    soldier->subState = BattleConfig::SUB_STATE_RUN;
    
    //check target in 5*5 tiles
    // 1 3 3
    // 1   0
    // 2 2 0
    
    static const float K[] = {
        1, 1, 0, 0,
        -1, 1, 0, 0,
        0, 0, 1, -1,
        0, 0, 1, 1,
    };
    
    for(int i = 1; i < 5; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            int j4 = j << 2;
            float a = K[j4 + 0];
            float b = K[j4 + 1];
            float c = K[j4 + 2];
            float d = K[j4 + 3];
            
            for(int z = 0; z < i * 2; z++)
            {
                int biasLine = (z & 1)? ((-z) >> 1): (z >> 1);
                
//                if((z & 1) == 0)
//                {
//                    biasLine = z / 2;
//                }
//                else
//                {
//                    biasLine = -(z + 1) / 2;
//                }
                
                int biasX = i * direction * a + biasLine * c;
                int biasZ = biasLine * b + i * d;

//                switch (j)
//                {
//                    case 0:
//                        biasX = i * direction;
//                        biasZ = biasLine;
//                        break;
//                        
//                    case 1:
//                        biasX = -i * direction;
//                        biasZ = biasLine;
//                        break;
//                        
//                    case 2:
//                        biasX = biasLine;
//                        biasZ = -i;
//                        break;
//                        
//                    case 3:
//                        biasX = biasLine;
//                        biasZ = i;
//                        break;
//                        
//                    default:
//                        break;
//                }
                
                int currentTileX = tileX + biasX;
                int currentTileZ = tileZ + biasZ;
                if(currentTileZ > -1 && currentTileZ < STAGE_HEIGHT && currentTileX > -1 && currentTileX < STAGE_WIDTH)
                {
                    //TO-DO if general
                    int targetId = m_entities->getTarget(currentTileZ, currentTileX);
                    if(targetId != 0 && MapHelper::getEntityType(targetId) == BattleConfig::ENTITY_SOLDIER)
                    {
                        auto pTargetEntity = m_entities->getEntity(targetId);
                        if (pTargetEntity != nullptr)
                        {
                            auto targetIdentify = pTargetEntity->component<BattleComponent::Identify>();
                            if (targetIdentify->side != identify->side)
                            {
                                soldier->target = targetIdentify->id;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return false;
    
    /*
    //check target in 5*STAGE_WIDTH
    int step = (soldier->side == BattleConfig::SIDE_LEFT) ? 1 : -1;
    
    biasX = 0;
    while (true)
    {
        biasX += step;
        if(tileX + biasX < 0 || tileX + biasX >= STAGE_WIDTH) break;
        
        for(int z = 0; z < 5; z++)
        {
            if(z % 2 == 0)
            {
                biasZ = z / 2;
            }
            else
            {
                biasZ = -(z + 1) / 2;
            }
            
            if(tileZ + biasZ > -1 && tileZ + biasZ < STAGE_HEIGHT)
            {
                if(m_entities->getTarget(tileZ + biasZ, tileX + biasX) != 0)
                {
                    //TO-DO if general
                    if(m_entities->getTarget(tileZ + biasZ, tileX + biasX) != 0 && MapHelper::getEntityType(m_entities->getTarget(tileZ + biasZ, tileX + biasX)) == BattleConfig::ENTITY_SOLDIER)
                    {
                        targetSoldier = m_entities->getSoldier(m_entities->getTarget(tileZ + biasZ, tileX + biasX));
                        
                        if(targetSoldier->side != soldier->side)
                        {
//                            targetSoldier->target = entity.component<BattleComponent::Identify>()->id;
                            soldier->target = targetSoldier->id;
                            return;
                        }
                    }
                }
            }
        }
    }
    
    auto soldiers = (soldier->side == BattleConfig::SIDE_LEFT) ? m_entities->getRightSoldiers() : m_entities->getLeftSoldiers();
    if(soldiers.size() == 0)
    {
        printf("End battle \n");
        soldier->cleanTarget();
//        soldier->state = BattleConfig::STATE_END;
    }
    else
    {
        soldier->target = soldiers[cocos2d::random() % soldiers.size()].component<BattleComponent::Identify>()->id;
    }
     */
    
}

void TargetFinder::findAllTarget()
{
    int tileZ, id;
    
    for(int i = STAGE_WIDTH - 1; i > -1; i--)
    {
        for(int j = 0; j < HALF_STAGE_HEIGHT; j++)
        {
            tileZ = j + HALF_STAGE_HEIGHT;
            id = m_entities->getTarget(tileZ, i);
            if(id != 0 && MapHelper::getEntityType(id) == BattleConfig::ENTITY_SOLDIER)
            {
                auto pTargetEntity = m_entities->getEntity(id);
                if (pTargetEntity != nullptr)
                {
                    auto targetSoldier = pTargetEntity->component<BattleComponent::Soldier>();
                    auto targetIdentify = pTargetEntity->component<BattleComponent::Identify>();
                    if (targetSoldier->target == 0 && targetIdentify->side == BattleConfig::SIDE_LEFT) findTarget(*pTargetEntity);// 10~19
                }
            }
            
            tileZ = HALF_STAGE_HEIGHT - 1 - j;
            id = m_entities->getTarget(tileZ, i);
            if(id != 0 && MapHelper::getEntityType(id) == BattleConfig::ENTITY_SOLDIER)
            {
                auto pTargetEntity = m_entities->getEntity(id);
                if (pTargetEntity != nullptr)
                {
                    auto targetSoldier = pTargetEntity->component<BattleComponent::Soldier>();
                    auto targetIdentify = pTargetEntity->component<BattleComponent::Identify>();
                    if (targetSoldier->target == 0 && targetIdentify->side == BattleConfig::SIDE_LEFT) findTarget(*pTargetEntity);// 9~0
                }
            }
        }
    }
    
    for(int i = 0; i < STAGE_WIDTH; i++)
    {
        for(int j = 0; j < HALF_STAGE_HEIGHT; j++)
        {
            tileZ = j + HALF_STAGE_HEIGHT;
            id = m_entities->getTarget(tileZ, i);
            if(id != 0 && MapHelper::getEntityType(id) == BattleConfig::ENTITY_SOLDIER)
            {
                auto pTargetEntity = m_entities->getEntity(id);
                if (pTargetEntity != nullptr)
                {
                    auto targetSoldier = pTargetEntity->component<BattleComponent::Soldier>();
                    auto targetIdentify = pTargetEntity->component<BattleComponent::Identify>();
                    if (targetSoldier->target == 0 && targetIdentify->side == BattleConfig::SIDE_RIGHT) findTarget(*pTargetEntity);// 10~19
                }
            }
            
            tileZ = HALF_STAGE_HEIGHT - 1 - j;
            id = m_entities->getTarget(tileZ, i);
            if(id != 0 && MapHelper::getEntityType(id) == BattleConfig::ENTITY_SOLDIER)
            {
                auto pTargetEntity = m_entities->getEntity(id);
                if (pTargetEntity != nullptr)
                {
                    auto targetSoldier = pTargetEntity->component<BattleComponent::Soldier>();
                    auto targetIdentify = pTargetEntity->component<BattleComponent::Identify>();
                    if (targetSoldier->target == 0 && targetIdentify->side == BattleConfig::SIDE_RIGHT) findTarget(*pTargetEntity);// 9~0
                }
            }
        }
    }
}

void TargetFinder::findCenter(int side, int size, BattleConfig::Tile& center)
{
    int gridWidth = STAGE_WIDTH / size;
    int gridHeight = STAGE_HEIGHT / size;
    
    int** grid = new int*[gridHeight];
    for(int i = 0; i < gridHeight; i++)
    {
        grid[i] = new int[gridWidth];
        for(int j = 0; j < gridWidth; j++)
        {
            grid[i][j] = 0;
        }
    }
    
    BattleComponent::Position::Handle pos;
    
    bool find = false;
    int x, z;
    std::vector<entityx::Entity> &soldiers = (side == BattleConfig::SIDE_LEFT) ? m_entities->getLeftSoldiers() : m_entities->getRightSoldiers();
    
    if (soldiers.size() == 0)
    {
        auto& heros = (side == BattleConfig::SIDE_LEFT ? m_entities->getLeftHeros() : m_entities->getRightHeros());
        if (heros.size() > 0)
        {
            pos = heros[cocos2d::random() % heros.size()].component<BattleComponent::Position>();
            center.tileZ = MapHelper::getTileZ(pos->z);
            center.tileX = MapHelper::getTileX(pos->x);
            find = true;
        }
    }
    
    if (!find)
    {
        for(int i = 0; i < soldiers.size(); i++)
        {
            pos = (soldiers[i]).component<BattleComponent::Position>();
            x = (pos->x / GRID_WIDTH) / size;
            z = (pos->z / GRID_HEIGHT) / size;
            if(x < gridWidth && z < gridHeight)
            {
                grid[z][x]++;
            }
        }
        
        int max = 0;
        for(int i = 0; i < gridHeight; i++)
        {
            for(int j = 0; j < gridWidth; j++)
            {
                if(max < grid[i][j])
                {
                    max = grid[i][j];
                }
            }
        }
        
        std::vector<int> all;
        
        for(int i = 0; i < gridHeight; i++)
        {
            for(int j = 0; j < gridWidth; j++)
            {
                if(max == grid[i][j])
                {
                    all.push_back((i << 8) | j);
                }
            }
        }
        
        int current = all[cocos2d::random() % all.size()];
        x = current & 0xFF;
        z = (current >> 8) & 0xFF;
        
        center.tileZ = z * size + size / 2;
        center.tileX = x * size + size / 2;
        find = true;
    }
    
    for(int i = 0; i < gridHeight; i++)
    {
        delete [] grid[i];
    }
    
    delete [] grid;
}

int TargetFinder::findHeroNearbyTarget(entityx::Entity& entity)
{
    auto general = entity.component<BattleComponent::General>();
    
    int frontTarget = findHeroFrontNearbyTarget(entity);
    int backTarget = findHeroBackNearbyTarget(entity);
    
    if (frontTarget > 0 && backTarget > 0)
    {
        general->target = (cocos2d::random() % 2 == 0 ? frontTarget : backTarget);
    }
    else if (frontTarget > 0)
    {
        general->target = frontTarget;
    }
    else if (backTarget > 0)
    {
        general->target = backTarget;
    }
    else
    {
        general->target = 0;
    }
    return general->target;
}

int TargetFinder::findHeroFrontNearbyTarget(entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();
    auto general = entity.component<BattleComponent::General>();
    
    // hero attack hero range, 0 = empty, * = hero, . = enemy
    //            0000..              ..0000
    // faceRight  00**.. or faceLeft  ..**00
    //            0000..              ..0000
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    
    /* lrui: comment out multi hero logic
    int tileXBias = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 2 : -3);
    int idx = 0;
    array<int, 3> targets = {0};
    bool hasEnemyHero = false;
    for (int tileZBias = -1; tileZBias <= 1; tileZBias++)
    {
        int targetId = findHeroTargetAtTile(entity, myTileZ + tileZBias, myTileX + tileXBias, TargetType::TARGET_HERO);
        if (targetId > 0) hasEnemyHero = true;
        targets[idx] = targetId;
        idx++;
    }
    
    if (hasEnemyHero)
    {
        int maxStandOrder = 0;
        int minTileDistance = STAGE_WIDTH;
        int firstAttackHeroId = 0;
        for (int targetId : targets)
        {
            if (targetId == 0) continue;
            
            auto pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity == nullptr) continue;
            
            int distance = abs(MapHelper::getTileX(pTargetEntity->component<BattleComponent::Position>()->x) - myTileX);
            int standOrder = pTargetEntity->component<BattleComponent::General>()->config.standOrder;
            if (distance < minTileDistance) minTileDistance = distance;
            if (standOrder > maxStandOrder) maxStandOrder = standOrder;
        
            if (distance > minTileDistance) continue;
            if (standOrder < maxStandOrder) continue;
            
            // 先攻击格子距离最近的敌将，如果格子距离一致，则先攻击站位排序最大的敌将
            firstAttackHeroId = targetId;
        }
        general->target = firstAttackHeroId;
    }
    else
    {
        tileXBias = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -2);
        // 只攻击正前方的敌兵
        general->target = findHeroTargetAtTile(entity, myTileZ, myTileX + tileXBias, TargetType::TARGET_SOLDIER);
    }*/
    
    int tileXBias = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -2);
    general->target = findHeroTargetAtTile(entity, myTileZ, myTileX + tileXBias, TargetType::TARGET_ALL);
    return general->target;
}

int TargetFinder::findHeroBackNearbyTarget(entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();
    auto general = entity.component<BattleComponent::General>();
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    int backTileX = myTileX + (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? -2 : 1);
    general->target = findHeroTargetAtTile(entity, myTileZ, backTileX, TargetType::TARGET_SOLDIER);
    return general->target;
}

int TargetFinder::findHeroBlockMoveTarget(entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    int myTileX = MapHelper::getTileX(position->x);
    int myTileZ = MapHelper::getTileZ(position->z);
    int targetId;
    
    if (direction->x > 0)
    {
        targetId = findHeroTargetAtTile(entity, myTileZ, myTileX + 1, TargetType::TARGET_SOLDIER);
        if (targetId > 0) return targetId;
    }
    if (direction->x < 0)
    {
        targetId = findHeroTargetAtTile(entity, myTileZ, myTileX - 2, TargetType::TARGET_SOLDIER);
        if (targetId > 0) return targetId;
    }
    if (direction->z > 0)
    {
        targetId = findHeroTargetAtTile(entity, myTileZ + 1, myTileX, TargetType::TARGET_SOLDIER);
        if (targetId > 0) return targetId;
        targetId = findHeroTargetAtTile(entity, myTileZ + 1, myTileX - 1, TargetType::TARGET_SOLDIER);
        if (targetId > 0) return targetId;
    }
    if (direction->z < 0)
    {
        targetId = findHeroTargetAtTile(entity, myTileZ - 1, myTileX, TargetType::TARGET_SOLDIER);
        if (targetId > 0) return targetId;
        targetId = findHeroTargetAtTile(entity, myTileZ - 1, myTileX - 1, TargetType::TARGET_SOLDIER);
        if (targetId > 0) return targetId;
    }
    
    return 0;
}

int TargetFinder::findHeroTargetAtTile(entityx::Entity& entity, int tileZ, int tileX, TargetType targetType)
{
    if (tileZ < 0 || tileZ >= STAGE_HEIGHT || tileX < 0 || tileX >= STAGE_WIDTH) return 0;
    
    auto identify = entity.component<BattleComponent::Identify>();
    
    int targetId = m_entities->getTarget(tileZ, tileX);
    if (targetId == 0) return 0;
    
    auto targetEntity = m_entities->getEntity(targetId);
    if (targetEntity == nullptr) return 0;

    auto targetIdentify = targetEntity->component<BattleComponent::Identify>();
    if (targetIdentify->side == identify->side) return 0;
    if (targetIdentify->type == BattleConfig::ENTITY_SOLDIER && targetType != TARGET_SOLDIER && targetType != TARGET_ALL) return 0;
    if (targetIdentify->type == BattleConfig::ENTITY_HERO && targetType != TARGET_HERO && targetType != TARGET_ALL) return 0;
    
    entity.component<BattleComponent::General>()->target = targetIdentify->id;
    return targetIdentify->id;
}

int TargetFinder::findHeroOpponentHero(entityx::Entity& entity)
{
    auto myIdentify = entity.component<BattleComponent::Identify>();
    auto myHero = entity.component<BattleComponent::General>();
    auto myPos = entity.component<BattleComponent::Position>();
    auto& enemyList = (myIdentify->side == BattleConfig::SIDE_LEFT ? _BATTLE_ENTITY.getRightHeros() : _BATTLE_ENTITY.getLeftHeros());
    for (auto& enemyEntity : enemyList)
    {
        if (isDead(enemyEntity)) continue;
        
        auto enemyPos = enemyEntity.component<BattleComponent::Position>();
        if (MapHelper::getTileZ(myPos->z) != MapHelper::getTileZ(enemyPos->z)) continue;
        
        // 身后的武将不考虑
        if (myIdentify->faceTo == BattleConfig::FACE_TO_LEFT && enemyPos->x > myPos->x) continue;
        if (myIdentify->faceTo == BattleConfig::FACE_TO_RIGHT && enemyPos->x < myPos->x) continue;
        
        int targetId = enemyEntity.component<BattleComponent::Identify>()->id;
        myHero->target = targetId;
        return targetId;
    }
    myHero->target = 0;
    return 0;
}

bool TargetFinder::findEnemyHeroOnSameLine(entityx::Entity& entity)
{
//    int tileZ = MapHelper::getTileZ(entity.component<BattleComponent::Position>()->z);
    float myZ = entity.component<BattleComponent::Position>()->z;
    
    if (entity.has_component<BattleComponent::TargetEntity>())
    {
        auto targetComp = entity.component<BattleComponent::TargetEntity>();
        auto& enemyEntity = targetComp->targetEntity;
        if (!isDead(enemyEntity) && enemyEntity.component<BattleComponent::Position>()->z == myZ)
        {
            return true;
        }
        else
        {
            targetComp.remove();
        }
    }
    
    auto identify = entity.component<BattleComponent::Identify>();
    auto& enemyList = (identify->side == BattleConfig::SIDE_LEFT ? _BATTLE_ENTITY.getRightHeros() : _BATTLE_ENTITY.getLeftHeros());
    for (auto& enemyEntity : enemyList)
    {
        if (!isDead(enemyEntity) && enemyEntity.component<BattleComponent::Position>()->z == myZ)
        {
            entity.assign<BattleComponent::TargetEntity>(enemyEntity);
            return true;
        }
    }
    
    return false;
}

void TargetFinder::findDiamondAreaTilesCenterOnHero(int radius, entityx::Entity& heroEntity, BattleConfig::TileResults& results)
{
    auto position = heroEntity.component<BattleComponent::Position>();
    int centerZ = MapHelper::getTileZ(position->z);
    int centerX = MapHelper::getTileX(position->x);
    
    // 0=empty .=soldier x=hero
    //00..00
    //0....0
    //..xx..
    //0....0
    //00..00
    static char s_heroFlag[ACTOR_ID_MAX % 1000];
    memset(s_heroFlag, 0, sizeof(s_heroFlag));
    
    int targetId, tileZ, tileX, flagId;
    
    for (int x = 0; x <= radius; x++)
    {
        for (int z = -radius; z <= radius; z++)
        {
            if (abs(x) + abs(z) <= radius)
            {
                tileZ = centerZ + z;
                tileX = centerX - 1 - x;
                if (MapHelper::validTile(tileZ, tileX))
                {
                    targetId = m_entities->getTarget(tileZ, tileX);
                    if (targetId > 0 && MapHelper::isHero(targetId))
                    {
                        flagId = targetId % 1000;
                        assert(flagId >= 0 && flagId < sizeof(s_heroFlag));
                        
                        if (s_heroFlag[flagId] == 0)
                        {
                            s_heroFlag[flagId] = 1;
                            results.add(tileZ, tileX);
                        }
                    }
                    else
                    {
                        results.add(tileZ, tileX);
                    }
                }
                tileX = centerX + x;
                if (MapHelper::validTile(tileZ, tileX))
                {
                    targetId = m_entities->getTarget(tileZ, tileX);
                    if (targetId > 0 && MapHelper::isHero(targetId))
                    {
                        flagId = targetId % 1000;
                        assert(flagId >= 0 && flagId < sizeof(s_heroFlag));
                        
                        if (s_heroFlag[flagId] == 0)
                        {
                            s_heroFlag[flagId] = 1;
                            results.add(tileZ, tileX);
                        }
                    }
                    else
                    {
                        results.add(tileZ, tileX);
                    }
                }
            }
        }
    }
}

void TargetFinder::findDiamondAreaTilesCenterOnTile(int radius, int tileZ, int tileX, BattleConfig::TileResults& results)
{
    // 0=empty .=soldier x=tile
    //00.00
    //0...0
    //..x..
    //0...0
    //00.00
    std::map<int, bool> heroIds;
    int targetId, tz, tx;
    
    for (int x = -radius; x <= radius; x++)
    {
        for (int z = -radius; z <= radius; z++)
        {
            if (abs(x) + abs(z) <= radius)
            {
                tz = tileZ + z;
                tx = tileX + x;
                
                if (MapHelper::validTile(tz, tx))
                {
                    targetId = m_entities->getTarget(tz, tx);
                    if (MapHelper::isHero(targetId))
                    {
                        if (heroIds.find(targetId) == heroIds.end())
                        {
                            results.add(tz, tx);
                            heroIds.insert(std::pair<int, bool>(targetId, true));
                        }
                    }
                    else
                    {
                        results.add(tz, tx);
                    }
                }
            }
        }
    }
}


void TargetFinder::findRectAreaTilesCenterOnTile(int radius, int tileZ, int tileX, BattleConfig::TileResults& results)
{
    // 0=empty .=soldier x=tile
    //00000
    //0...0
    //0.x.0
    //0...0
    //00000
    
    static char s_heroFlag[ACTOR_ID_MAX % 1000];
    memset(s_heroFlag, 0, sizeof(s_heroFlag));
    
    int targetId, tz, tx, flagId;
    
    for (int x = -radius; x <= radius; x++)
    {
        for (int z = -radius; z <= radius; z++)
        {
            tz = tileZ + z;
            tx = tileX + x;
            
            if (MapHelper::validTile(tz, tx))
            {
                targetId = m_entities->getTarget(tz, tx);
                if (targetId > 0 && MapHelper::isHero(targetId))
                {
                    flagId = targetId % 1000;
                    assert(flagId >= 0 && flagId < sizeof(s_heroFlag));

                    if (s_heroFlag[flagId] == 0)
                    {
                        s_heroFlag[flagId] = 1;
                        results.add(tz, tx);
                    }
                }
                else
                {
                    results.add(tz, tx);
                }
            }
        }
    }
}

void TargetFinder::findRectAreaTilesOnHero(int width, int height, entityx::Entity& heroEntity, BattleConfig::TileResults& results, bool reverse)
{
    // 0=empty .=soldier x=tile
    //....  ....
    //....  ....
    //....xx....
    //....  ....
    //....  ....
    
    static std::array<int, (ACTOR_ID_MAX % 1000)> s_heroFlag;
    s_heroFlag.fill(0);
    
    auto faceTo = heroEntity.component<BattleComponent::Identify>()->faceTo;
    auto position = heroEntity.component<BattleComponent::Position>();
    int startX;
    if(reverse)
    {
        startX = MapHelper::getTileX(position->x) + (faceTo == BattleConfig::FACE_TO_RIGHT ? (-1 - width) : 1);
    }
    else
    {
        startX = MapHelper::getTileX(position->x) + (faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : (-1 - width));
    }
    int startZ = MapHelper::getTileZ(position->z) - height / 2;
    int tileX, tileZ, targetId, flagId;
    for(int z = 0; z < height; z++)
    {
        for(int x = 0; x < width; x++)
        {
            tileZ = startZ + z;
            tileX = startX + x;
            
            if (MapHelper::validTile(tileZ, tileX))
            {
                targetId = m_entities->getTarget(tileZ, tileX);
                if (targetId > 0 && MapHelper::isHero(targetId))
                {
                    flagId = targetId % 1000;
                    if (s_heroFlag[flagId] == 0)
                    {
                        s_heroFlag[flagId] = 1;
                        results.add(tileZ, tileX);
                    }
                }
                else
                {
                    results.add(tileZ, tileX);
                }
            }
        }
    }
}

void TargetFinder::findRectAreaArroundHero(int width, int height, entityx::Entity& heroEntity, BattleConfig::TileResults& results)
{
    // 0=empty .=soldier x=tile
    //..........
    //..........
    //....xx....
    //..........
    //..........
    
    static std::array<int, (ACTOR_ID_MAX % 1000)> s_heroFlag;
    s_heroFlag.fill(0);
    
    auto position = heroEntity.component<BattleComponent::Position>();
    int startX = MapHelper::getTileX(position->x) - (width + 1) / 2;
    int startZ = MapHelper::getTileZ(position->z) - height / 2 + 1;
    int tileX, tileZ, targetId, flagId;

    for(int z = 0; z < height; z++)
    {
        for(int x = 0; x < width; x++)
        {
            tileZ = startZ + z;
            tileX = startX + x;
            
            if (MapHelper::validTile(tileZ, tileX))
            {
                targetId = m_entities->getTarget(tileZ, tileX);
                if (targetId > 0 && MapHelper::isHero(targetId))
                {
                    flagId = targetId % 1000;
                    if (s_heroFlag[flagId] == 0)
                    {
                        s_heroFlag[flagId] = 1;
                        results.add(tileZ, tileX);
                    }
                }
                else
                {
                    results.add(tileZ, tileX);
                }
            }
        }
    }
}

void TargetFinder::findRectAreaArroundTile(int width, int height, int centerX, int centerZ, BattleConfig::TileResults& results)
{
    // 0=empty .=soldier x=tile
    //..........
    //..........
    //....xx....
    //..........
    //..........
    
    static std::array<int, (ACTOR_ID_MAX % 1000)> s_heroFlag;
    s_heroFlag.fill(0);
    
    int startX = centerX - (width + 1) / 2;
    int startZ = centerZ - height / 2 + 1;
    int tileX, tileZ, targetId, flagId;
    
    for(int z = 0; z < height; z++)
    {
        for(int x = 0; x < width; x++)
        {
            tileZ = startZ + z;
            tileX = startX + x;
            
            if (MapHelper::validTile(tileZ, tileX))
            {
                targetId = m_entities->getTarget(tileZ, tileX);
                if (targetId > 0 && MapHelper::isHero(targetId))
                {
                    flagId = targetId % 1000;
                    if (s_heroFlag[flagId] == 0)
                    {
                        s_heroFlag[flagId] = 1;
                        results.add(tileZ, tileX);
                    }
                }
                else
                {
                    results.add(tileZ, tileX);
                }
            }
        }
    }
}

void TargetFinder::findSkillTargets(int fromHeroId, BattleConfig::SkillTarget skillTarget, TargetResults& results)
{
    switch (skillTarget)
    {
        case BattleConfig::SkillTarget::SELF_HERO:
        {
            results.add(fromHeroId);
            break;
        }
        case BattleConfig::SkillTarget::OPP_HERO:
        {
            auto* pFromEntity = _BATTLE_ENTITY.getEntity(fromHeroId);
            if (pFromEntity != nullptr)
            {
                int targetId = findHeroOpponentHero(*pFromEntity);
                if (targetId > 0) results.add(targetId);
            }
            break;
        }
        case BattleConfig::SkillTarget::SELF_SOLDIER:
        {
            BattleConfig::Side selfSide = MapHelper::getSide(fromHeroId);
            auto& soldiers = (selfSide == BattleConfig::SIDE_LEFT ? _BATTLE_ENTITY.getLeftSoldiers() : _BATTLE_ENTITY.getRightSoldiers());
            for (auto& entity : soldiers)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            break;
        }
        case BattleConfig::SkillTarget::OPP_SOLDIER:
        {
            BattleConfig::Side selfSide = MapHelper::getSide(fromHeroId);
            auto& soldiers = (selfSide == BattleConfig::SIDE_RIGHT ? _BATTLE_ENTITY.getLeftSoldiers() : _BATTLE_ENTITY.getRightSoldiers());
            for (auto& entity : soldiers)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            break;
        }
        case BattleConfig::SkillTarget::SELF_HERO_ALL:
        {
            BattleConfig::Side selfSide = MapHelper::getSide(fromHeroId);
            auto& heros = (selfSide == BattleConfig::SIDE_LEFT ? _BATTLE_ENTITY.getLeftHeros() : _BATTLE_ENTITY.getRightHeros());
            for (auto& entity : heros)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            break;
        }
        case BattleConfig::SkillTarget::OPP_HERO_ALL:
        {
            BattleConfig::Side selfSide = MapHelper::getSide(fromHeroId);
            auto& heros = (selfSide == BattleConfig::SIDE_LEFT ? _BATTLE_ENTITY.getRightHeros() : _BATTLE_ENTITY.getLeftHeros());
            for (auto& entity : heros)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            break;
        }
        case BattleConfig::SkillTarget::SELF_ALL:
        {
            BattleConfig::Side selfSide = MapHelper::getSide(fromHeroId);
            auto& heros = (selfSide == BattleConfig::SIDE_LEFT ? _BATTLE_ENTITY.getLeftHeros() : _BATTLE_ENTITY.getRightHeros());
            for (auto& entity : heros)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            auto& soldiers = (selfSide == BattleConfig::SIDE_LEFT ? _BATTLE_ENTITY.getLeftSoldiers() : _BATTLE_ENTITY.getRightSoldiers());
            for (auto& entity : soldiers)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            break;
        }
        case BattleConfig::SkillTarget::OPP_ALL:
        {
            BattleConfig::Side selfSide = MapHelper::getSide(fromHeroId);
            auto& heros = (selfSide == BattleConfig::SIDE_RIGHT ? _BATTLE_ENTITY.getLeftHeros() : _BATTLE_ENTITY.getRightHeros());
            for (auto& entity : heros)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            auto& soldiers = (selfSide == BattleConfig::SIDE_RIGHT ? _BATTLE_ENTITY.getLeftSoldiers() : _BATTLE_ENTITY.getRightSoldiers());
            for (auto& entity : soldiers)
            {
                results.add(entity.component<BattleComponent::Identify>()->id);
            }
            break;
        }
        case BattleConfig::SkillTarget::FRONT_NEARBY_ENEMY:
        {
            auto* pFromEntity = _BATTLE_ENTITY.getEntity(fromHeroId);
            if (pFromEntity != nullptr)
            {
                int targetId = findHeroFrontNearbyTarget(*pFromEntity);
                if (targetId > 0) results.add(targetId);
            }
            break;
        }
        default:
            break;
    }
}


bool TargetFinder::findBowmanTarget(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    
    bool r = false;
    if(identify->side == BattleConfig::SIDE_LEFT)
    {
        r = findBowmanTargetRightward(entity);
        if(!r)
        {
            r = findBowmanTargetLeftward(entity);
        }
    }
    else if(identify->side == BattleConfig::SIDE_RIGHT)
    {
        r = findBowmanTargetLeftward(entity);
        if(!r)
        {
            r = findBowmanTargetRightward(entity);
        }
        
    }
    
    return r;
    
}

bool TargetFinder::findBowmanTargetRightward(entityx::Entity& entity)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();

    int tileZ = MapHelper::getTileZ(position->z);
    int tileX = MapHelper::getTileX(position->x);

    
    for(int i = tileX + 1; i < STAGE_WIDTH; ++i)
    {
        auto targetId = _BATTLE_ENTITY.getTarget(tileZ, i);
        auto targetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if(targetEntity != NULL)
        {
            auto targetIdentify = targetEntity->component<BattleComponent::Identify>();
            if(targetIdentify->side != identify->side)
            {
                soldier->target = targetId;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    
//    soldier->target = 0;
    return false;
}

bool TargetFinder::findBowmanTargetLeftward(entityx::Entity& entity)
{
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto position = entity.component<BattleComponent::Position>();
    auto identify = entity.component<BattleComponent::Identify>();
    
    int tileZ = MapHelper::getTileZ(position->z);
    int tileX = MapHelper::getTileX(position->x);
    
    
    for(int i = tileX - 1; i >= 0; --i)
    {
        auto targetId = _BATTLE_ENTITY.getTarget(tileZ, i);
        auto targetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if(targetEntity != NULL)
        {
            auto targetIdentify = targetEntity->component<BattleComponent::Identify>();
            if(targetIdentify->side != identify->side)
            {
                soldier->target = targetId;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    
//    soldier->target = 0;
    return false;
    
}

//{
//    auto position = entity.component<BattleComponent::Position>();
//    int tileZ = MapHelper::getTileZ(position->z);
//    int tileX = MapHelper::getTileX(position->x);
//    
//    auto identify = entity.component<BattleComponent::Identify>();
//    int cache = getCheckInLineCache(identify->side, tileZ);
//    if(cache != CHECK_TARGET_INLINE_NO_CACHE)
//    {
//        return cache;
//    }
//
//    int value = 0;
//    for(int i = 0; i < STAGE_WIDTH; ++i)
//    {
//        auto targetId = _BATTLE_ENTITY.getTarget(tileZ, i);
//        auto targetEntity = _BATTLE_ENTITY.getEntity(targetId);
//        if(targetEntity != NULL)
//        {
//            auto targetIdentify = targetEntity->component<BattleComponent::Identify>();
//            if(targetIdentify->side != identify->side)
//            {
//                value = (i < tileX)? -1: 1;
//                break;
//            }
//        }
//    }
//    
//    
//    setCheckInLineCache(identify->side, tileZ, value);
//    return value;
//    
//}

//void TargetFinder::cleanCheckTargetInLine()
//{
//    memset(m_checkTargetInLineCacheLeft, CHECK_TARGET_INLINE_NO_CACHE, sizeof(m_checkTargetInLineCacheLeft));
//    memset(m_checkTargetInLineCacheRight, CHECK_TARGET_INLINE_NO_CACHE, sizeof(m_checkTargetInLineCacheLeft));
//}
//
//int TargetFinder::getCheckInLineCache(int side, int tileZ)
//{
//    if(tileZ >= 0 && tileZ < STAGE_HEIGHT)
//    {
//        if(side == BattleConfig::SIDE_LEFT)
//        {
//            return m_checkTargetInLineCacheLeft[tileZ];
//        }
//        else if(side == BattleConfig::SIDE_RIGHT)
//        {
//            return m_checkTargetInLineCacheRight[tileZ];
//        }
//        
//    }
//    
//    return CHECK_TARGET_INLINE_NO_CACHE;
//
//}
//
//void TargetFinder::setCheckInLineCache(int side, int tileZ, int value)
//{
//    if(tileZ >= 0 && tileZ < STAGE_HEIGHT)
//    {
//        if(side == BattleConfig::SIDE_LEFT)
//        {
//            m_checkTargetInLineCacheLeft[tileZ] = value;
//        }
//        else if(side == BattleConfig::SIDE_RIGHT)
//        {
//            m_checkTargetInLineCacheRight[tileZ] = value;
//        }
//        
//    }
//
//}

