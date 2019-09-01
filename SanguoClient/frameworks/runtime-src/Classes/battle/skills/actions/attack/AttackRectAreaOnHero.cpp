//
//  AttackRectAreaOnHero.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/14/15.
//
//

#include "AttackRectAreaOnHero.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AttackRectAreaOnHero::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int width = (int)m_skillActionData->config["width"]->d;
    int height = (int)m_skillActionData->config["height"]->d;
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
        if (skillTargetResults.valid()) targets = skillTargetResults->targets;
    }
    
    for (int targetId : targets)
    {
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if (pTargetEntity == nullptr) continue;
        
        BattleConfig::TileResults results;
        if(m_skillActionData->hasMember("reverse") && m_skillActionData->config["reverse"]->b)
        {
            _TARGET_FINDER.findRectAreaTilesOnHero(width, height, *pTargetEntity, results, true);
        }
        else
        {
            _TARGET_FINDER.findRectAreaTilesOnHero(width, height, *pTargetEntity, results);
        }
        
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
            attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH - distance * m_skillActionData->damageData.deadSpeedHDecay;
            attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV - distance * m_skillActionData->damageData.deadSpeedVDecay;
            attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
            
            _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
        }
    }
    
    remove();
}
