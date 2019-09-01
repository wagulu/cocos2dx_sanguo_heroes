//
//  AttackRectArroundHero.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/8/15.
//
//

#include "AttackRectArroundHero.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AttackRectArroundHero::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int width = (int)m_skillActionData->config["width"]->d;
    int height = (int)m_skillActionData->config["height"]->d;
    float spreadInterval = m_skillActionData->config["spreadInterval"]->d;
    
    auto skillTarget = EnumParserSkillTarget().getValue(*m_skillActionData->config["SkillTarget"]->s);
    TargetFinder::TargetResults targetResults;
    _TARGET_FINDER.findSkillTargets(m_fromId, skillTarget, targetResults);
    
    for (int targetId : targetResults.targets)
    {
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if (pTargetEntity == nullptr) continue;
        
        BattleConfig::TileResults results;
        
        _TARGET_FINDER.findRectAreaArroundHero(width, height, *pTargetEntity, results);
        
        auto position = pTargetEntity->component<BattleComponent::Position>();
        int centerZ = MapHelper::getTileZ(position->z);
        int centerX = MapHelper::getTileX(position->x);
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
