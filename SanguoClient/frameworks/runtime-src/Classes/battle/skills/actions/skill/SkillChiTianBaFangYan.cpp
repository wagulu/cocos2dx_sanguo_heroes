//
//  SkillChiTianBaFangYan.cpp
//  Game
//
//  Created by fuchenhao on 3/6/15.
//
//

#include "SkillChiTianBaFangYan.h"
#include "BattleWorld.h"
#include "StateSoldierChangeToGhost.h"

using namespace entityx;

void SkillChiTianBaFangYan::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto skillTargetResults = es.get(m_entityId).component<BattleComponent::SkillTargetResults>();
    if (skillTargetResults.valid())
    {
        for (int targetId : skillTargetResults->targets)
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity == nullptr) continue;
            
            float radius = m_skillActionData->config["radius"]->d;
            
            BattleConfig::TileResults results;
            _TARGET_FINDER.findDiamondAreaTilesCenterOnHero(radius, *pTargetEntity, results);
            
            BattleConfig::AttackData attackHeroData(m_fromId);
            attackHeroData.setTargetId(targetId);
            attackHeroData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
            
            StateSoldierChangeToGhost::GhostData ghostData(attackHeroData);
            ghostData.targetId = targetId;
            ghostData.flyUpTime = m_skillActionData->config["ghostFlyUpTime"]->d;
            ghostData.flyUpHeight = m_skillActionData->config["ghostFlyUpHeight"]->d;
            ghostData.flyUpStartSpeed = m_skillActionData->config["ghostFlyUpStartSpeed"]->d;
            ghostData.flyUpEndSpeed = m_skillActionData->config["ghostFlyUpEndSpeed"]->d;
            ghostData.flyUpFactorX = m_skillActionData->config["ghostFlyUpFactorX"]->d;
            ghostData.scaleFactor = m_skillActionData->config["ghostScaleFactor"]->d;
            ghostData.scaleMax = m_skillActionData->config["ghostScaleMax"]->d;
            ghostData.changeToGhostTime = m_skillActionData->config["ghostChangeTime"]->d;
            ghostData.moveAwayTime = m_skillActionData->config["ghostMoveAwayTime"]->d;
            ghostData.moveAwayDistance = m_skillActionData->config["ghostMoveAwayDistance"]->d;
            ghostData.moveCloseTime = m_skillActionData->config["ghostMoveCloseTime"]->d;
            ghostData.hitOffset = m_skillActionData->config["ghostHitOffset"]->d;
            ghostData.appearEffect = m_effect->getEffect("soldier_to_ghost");
            ghostData.loopEffect = m_effect->getEffect("ghost");
            ghostData.hitEffect = m_effect->getEffect("ghost_hit_hero");
            
            auto position = pTargetEntity->component<BattleComponent::Position>();
            int centerZ = MapHelper::getTileZ(position->z);
            int centerX = MapHelper::getTileX(position->x);
            float minDistance = -1;
            float maxDistance = -1;
            vector<entityx::Entity*> soldiers;
            vector<float> soldiersDistance;
            vector<int> soldierIds;
            
            for (auto* pTile : results.tiles)
            {
                int tileTargetId = _BATTLE_ENTITY.getTarget(pTile->tileZ, pTile->tileX);
                if (!MapHelper::isSoldier(tileTargetId)) continue;
                
                auto* pSoldierEntity = _BATTLE_ENTITY.getEntity(tileTargetId);
                if (pSoldierEntity == nullptr) continue;
                
                float distance = abs(pTile->tileZ - centerZ) + abs(pTile->tileX - centerX);
                if (minDistance == -1 || distance < minDistance) minDistance = distance;
                if (maxDistance == -1 || distance > maxDistance) maxDistance = distance;
                
                soldiers.push_back(pSoldierEntity);
                soldiersDistance.push_back(distance);
                soldierIds.push_back(tileTargetId);
            }
            
            float heroFlyUpTime = 0;
            int firstHitSoldierId = 0;
            int lastHitSoldierId = 0;
            auto numSoldiers = soldiers.size();
            if (numSoldiers > 0)
            {
                float lastHitDelay = m_skillActionData->config["lastHitDelay"]->d;
                float hitTotalTime = m_skillActionData->config["hitTotalTime"]->d;
                float hitIntervalMax = m_skillActionData->config["hitIntervalMax"]->d;
                float speedFactor = m_skillActionData->config["ghostFlyUpSpeedFactor"]->d;
                
                float hitInterval = min(hitTotalTime / (float)numSoldiers, hitIntervalMax);
                float hitDelay = hitInterval;
                
                for (int i = 0; i < numSoldiers; i++)
                {
                    auto* pSoldierEntity = soldiers[i];
                    float distance = soldiersDistance[i];
                    
                    StateSoldierChangeToGhost::GhostData finalGhostData = ghostData;
                    
                    if (distance > 0)
                    {
                        finalGhostData.flyUpStartSpeed *= 1.0 - (1.0f / distance) * speedFactor;
                    }
                    
                    if (distance == maxDistance)
                    {
                        finalGhostData.isLastHit = true;
                        finalGhostData.flyUpTime += hitInterval * numSoldiers + lastHitDelay;
                        finalGhostData.hitEffect = m_effect->getEffect("ghost_hit_hero_critical");
                        
                        maxDistance = -1;
                        lastHitSoldierId = pSoldierEntity->component<BattleComponent::Identify>()->id;
                    }
                    else if (distance == minDistance)
                    {
                        finalGhostData.isFirstHit = true;
                        finalGhostData.flyUpTime += 0;
                        
                        minDistance = -1;
                        firstHitSoldierId = pSoldierEntity->component<BattleComponent::Identify>()->id;
                    }
                    else
                    {
                        finalGhostData.flyUpTime += hitDelay;
                        hitDelay += hitInterval;
                    }
                    
                    if (finalGhostData.isFirstHit || (finalGhostData.isLastHit && numSoldiers == 1))
                    {
                        heroFlyUpTime = finalGhostData.flyUpTime + finalGhostData.changeToGhostTime + finalGhostData.moveAwayTime + finalGhostData.moveCloseTime;
                    }
                    
                    _ENTITY_EVENT.emit(BattleEvent::StrategySoldierChangeToGhost(*pSoldierEntity, finalGhostData));
                }
            }
        
            BattleConfig::HeroDizzyData dizzyData;
            dizzyData.skillEntityId = m_entityId;
            
            BattleConfig::HeroDizzyConfig dizzyConfig = *m_skillData->pConfig->dizzy->getConfig(*m_skillActionData->config["heroDizzyConfig"]->s);
            
            auto* pDizzyActionSkillChiTian = dizzyConfig.findAction("skill_chi_tian_ba_fang_yan");
            if (pDizzyActionSkillChiTian != nullptr)
            {
                pDizzyActionSkillChiTian->config["firstHitSoldierId"] = BattleConfig::ParamData((double)firstHitSoldierId);
                pDizzyActionSkillChiTian->config["lastHitSoldierId"] = BattleConfig::ParamData((double)lastHitSoldierId);
                
                if (heroFlyUpTime > 0)
                {
                    pDizzyActionSkillChiTian->config["flyUpTime"] = BattleConfig::ParamData((double)heroFlyUpTime);
                }
                
                _ENTITY_EVENT.emit(BattleEvent::StrategyHeroDizzy(*pTargetEntity, dizzyData, dizzyConfig));
            }
            
            auto* pEffectHeroFire = m_effect->getEffect("hero_fire");
            bool reserse = pTargetEntity->component<BattleComponent::Identify>()->faceTo == BattleConfig::FACE_TO_RIGHT;
            _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(pEffectHeroFire, reserse, *pTargetEntity, 0, heroFlyUpTime, m_fromId);
            
            if (numSoldiers > 0)
            {
                skillTargetResults->targets = soldierIds;
                
                _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "kill_soldier");
            }
        }
    }
    
    remove();
}
