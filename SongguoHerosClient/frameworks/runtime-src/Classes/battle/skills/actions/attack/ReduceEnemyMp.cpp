//
//  ReduceEnemyMp.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/21/15.
//
//

#include "ReduceEnemyMp.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void ReduceEnemyMp::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pFromEntity != nullptr)
    {
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
        
        float totalMpReduced = 0;
        float hurtMP = ceil(caclFormulaResult(*m_skillActionData->config["formula"]->s));
        if (hurtMP > 0)
        {
            for (int targetId : targets)
            {
                auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
                if (pTargetEntity == nullptr) continue;
                
                auto targetProperty = pTargetEntity->component<BattleComponent::Property>();
                
                float maxMP = targetProperty->getOriginal(BattleConfig::Property::MP);
                float curMP = targetProperty->get(BattleConfig::Property::MP);
                float mp = curMP - hurtMP;
                if (mp > maxMP) mp = maxMP;
                if (mp < 0) mp = 0;
                targetProperty->set(BattleConfig::Property::MP, mp);
                
                float actualMpChanged = mp - curMP;
                
                _EFFECT_CREATOR.createMPChangeEffect(*pFromEntity, *pTargetEntity, actualMpChanged, false, false);
                
                if (actualMpChanged < 0)
                {
                    totalMpReduced += abs(actualMpChanged);
                }
            }
        }
        
        auto skillEntity = _ENTITY_MANAGER.get(m_entityId);
        auto skillComp = skillEntity.component<BattleComponent::SkillReduceEnemyMp>();
        if (skillComp.valid())
        {
            skillComp.remove();
        }
        skillEntity.assign<BattleComponent::SkillReduceEnemyMp>(totalMpReduced);
    }
    
    remove();
}
