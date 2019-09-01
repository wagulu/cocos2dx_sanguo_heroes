//
//  AttackDiamondAreaCenterOnTile.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/10/15.
//
//

#include "AttackDiamondAreaCenterOnTile.h"

#include "BattleWorld.h"

void AttackDiamondAreaCenterOnTile::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int radius = m_skillActionData->config["radius"]->d;
    float spreadInterval = m_skillActionData->config["spreadInterval"]->d;
    
    auto center = es.get(m_entityId).component<BattleComponent::SkillTileTarget>();
    int centerZ = center->tileZ;
    int centerX = center->tileX;
    int deltaX;
    int deltaZ;
    int distance;
    
    BattleConfig::TileResults results;
    float x = MapHelper::getCenterX(center->tileX, false);
    float z = MapHelper::getCenterZ(center->tileZ);
    
    _TARGET_FINDER.findDiamondAreaTilesCenterOnTile(radius, center->tileZ, center->tileX, results);
    
    for (auto* pTile : results.tiles)
    {
        deltaX = abs(pTile->tileX - centerX);
        deltaZ = abs(pTile->tileZ - centerZ);
        distance = max(deltaX, deltaZ) - 1;
        // check  未分敌我
        BattleConfig::AttackData attackData(m_fromId, (deltaX + deltaZ) * spreadInterval);
        attackData.setTargetPos(pTile->tileZ, pTile->tileX);
        attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
        attackData.damagePosition.x = x;
        attackData.damagePosition.y = 0;
        attackData.damagePosition.z = z;
        attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
        if (distance < 0)
        {
            attackData.damagePosition.speedH = 0;
            attackData.damagePosition.speedV = 0;
        }
        else
        {
            attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH - distance * m_skillActionData->damageData.deadSpeedHDecay;
            attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV - distance * m_skillActionData->damageData.deadSpeedVDecay;
        }
        if (m_skillActionData->damageData.deadAnimation)
        {
            attackData.damagePosition.group = m_effect->getEffect(*m_skillActionData->damageData.deadAnimation);
        }
        
        _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    }
    
    remove();
}
