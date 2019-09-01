//
//  AttackRectArroundTile.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/25/15.
//
//

#include "AttackRectArroundTile.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AttackRectArroundTile::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int width = (int)m_skillActionData->config["width"]->d;
    int height = (int)m_skillActionData->config["height"]->d;
    float spreadInterval = m_skillActionData->config["spreadInterval"]->d;
    
    auto center = es.get(m_entityId).component<BattleComponent::SkillTileTarget>();
    int centerZ = center->tileZ;
    int centerX = center->tileX;
    
    BattleConfig::TileResults results;
    
    _TARGET_FINDER.findRectAreaArroundTile(width, height, centerX, centerZ, results);
    
    int deltaX;
    int deltaZ;
    int distance;
    bool randomDelay = m_skillActionData->hasMember("randomDelay") && m_skillActionData->config["randomDelay"]->b;
    float delay;
    
    for (auto* pTile : results.tiles)
    {
        deltaX = abs(pTile->tileX - centerX);
        deltaZ = abs(pTile->tileZ - centerZ);
        distance = max(deltaX, deltaZ) - 1;
        
        delay = randomDelay ? cocos2d::rand_0_1() * 0.5 : (deltaX + deltaZ) * spreadInterval;
        BattleConfig::AttackData attackData(m_fromId, delay);
        attackData.setTargetPos(pTile->tileZ, pTile->tileX);
        attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
        attackData.damagePosition.x = centerX * GRID_WIDTH + HALF_GRID_WIDTH;
        attackData.damagePosition.y = 0;
        attackData.damagePosition.z = centerZ * GRID_HEIGHT + HALF_GRID_HEIGHT;
        attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH - distance * m_skillActionData->damageData.deadSpeedHDecay;
        attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV - distance * m_skillActionData->damageData.deadSpeedVDecay;
        attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
        
        _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    }

    remove();
}
