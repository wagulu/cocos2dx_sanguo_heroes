//
//  AttackDiamondAreaCenterOnHero.cpp
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#include "AttackDiamondAreaCenterOnHero.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AttackDiamondAreaCenterOnHero::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int radius = m_skillActionData->config["radius"]->d;
    float spreadInterval = m_skillActionData->config["spreadInterval"]->d;
    
    TargetFinder::TargetResults targetResults;
    vector<int>& targets = targetResults.targets;
    if (m_skillActionData->hasMember("SkillTarget"))
    {
        auto skillTarget = EnumParserSkillTarget().getValue(*m_skillActionData->config["SkillTarget"]->s);
        _TARGET_FINDER.findSkillTargets(m_fromId, skillTarget, targetResults);
    }
    else
    {
        auto skillTargetResults = es.get(m_entityId).component<BattleComponent::SkillTargetResults>();
        if (skillTargetResults.valid())
        {
            targets = skillTargetResults->targets;
        }
    }
    
    for (int targetId : targets)
    {
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if (pTargetEntity == nullptr) continue;
    
        BattleConfig::TileResults results;
        
        _TARGET_FINDER.findDiamondAreaTilesCenterOnHero(radius, *pTargetEntity, results);
        
        auto position = pTargetEntity->component<BattleComponent::Position>();
        int centerZ = MapHelper::getTileZ(position->z);
        int centerX = MapHelper::getTileX(position->x);
        int deltaX;
        int deltaZ;
        int distance;
        
        for (auto* pTile : results.tiles)
        {
            deltaX = abs(pTile->tileX - centerX);
            deltaZ = abs(pTile->tileZ - centerZ);
            distance = max(deltaX, deltaZ) - 1;
            
            BattleConfig::AttackData attackData(m_fromId, (deltaX + deltaZ) * spreadInterval);
            attackData.setTargetPos(pTile->tileZ, pTile->tileX);
            attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
            attackData.damagePosition.x = position->x;
            attackData.damagePosition.y = 0;
            attackData.damagePosition.z = position->z;
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
            attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
            
            _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
        }
    }
    
    remove();
}
