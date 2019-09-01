//
//  FindTargetByFormation.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/25/15.
//
//

#include "FindTargetByFormation.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void FindTargetByFormation::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto skillEntity = _ENTITY_MANAGER.get(m_entityId);
    auto skillTargetResult = skillEntity.component<BattleComponent::SkillTargetResults>();
    if (skillTargetResult.valid())
    {
        skillTargetResult.remove();
    }
    
    //TODO, lrui, multi general
    int targetId = LEFT_HERO;
    if(m_side == BattleConfig::SIDE_LEFT)
    {
        targetId = RIGHT_HERO;
    }
    
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity != nullptr)
    {
        int tileX = MapHelper::getTileX(pTargetEntity->component<BattleComponent::Position>()->x);
        
        TargetFinder::TargetResults targetResults;
        
        std::string formation = "c2";
        if (m_skillActionData->config["formation"])
        {
            formation = *m_skillActionData->config["formation"]->s;
        }
        
        BattleConfig::FormationConfig& formationConfig = FormationManager::getInstance()->getFormation(formation);
        
        int xPosBaseDelta = 0;
        if (m_skillActionData->config["dtX"])
        {
            xPosBaseDelta = m_skillActionData->config["dtX"]->d;
        }
        
        int callNum = formationConfig.validSoldiersNum;
        int tarSide = 1;
        if(m_side == BattleConfig::SIDE_LEFT)
        {
            tileX -= xPosBaseDelta;
        }
        else
        {
            tileX += xPosBaseDelta;
            tarSide = -1;
        }
        
        for(int i= 0; i < callNum; i++)
        {
            auto& pos = formationConfig.getSoldierPos(i);
            auto targetId = _BATTLE_ENTITY.getTarget(pos.z, tileX + tarSide * pos.x);
            targetResults.add(targetId);
            
            auto targetId2 = _BATTLE_ENTITY.getTarget(pos.z, tileX + tarSide * pos.x + 1);
            targetResults.add(targetId2);
            
            auto targetId3 = _BATTLE_ENTITY.getTarget(pos.z, tileX + tarSide * pos.x - 1);
            targetResults.add(targetId3);
            
            auto targetId4 = _BATTLE_ENTITY.getTarget(pos.z + 1, tileX + tarSide * pos.x);
            targetResults.add(targetId4);
            
            auto targetId5 = _BATTLE_ENTITY.getTarget(pos.z - 1, tileX + tarSide * pos.x);
            targetResults.add(targetId5);
        }
        
        skillEntity.assign<BattleComponent::SkillTargetResults>(targetResults.targets);
    }
    
    remove();
}
