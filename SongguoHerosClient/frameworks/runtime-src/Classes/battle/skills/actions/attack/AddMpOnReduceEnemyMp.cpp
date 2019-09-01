//
//  AddMpOnReduceEnemyMp.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/21/15.
//
//

#include "AddMpOnReduceEnemyMp.h"
#include "BattleWorld.h"

void AddMpOnReduceEnemyMp::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    do
    {
        auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
        if (pFromEntity == nullptr) break;
        
        auto skillEntity = es.get(m_entityId);
        if (!skillEntity.valid()) break;
        
        auto skillTargetResults = skillEntity.component<BattleComponent::SkillTargetResults>();
        if (!skillTargetResults.valid()) break;
        
        auto skillReduceEnemyMp = skillEntity.component<BattleComponent::SkillReduceEnemyMp>();
        if (!skillReduceEnemyMp.valid()) break;
        
        float percentBaseOnReduce = m_skillActionData->config["percentBaseOnReduce"]->d;
        float mpAdded = percentBaseOnReduce * skillReduceEnemyMp->actualReduceValue;
        
        for (int targetId : skillTargetResults->targets)
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity == nullptr) continue;
            
            auto targetProperty = pTargetEntity->component<BattleComponent::Property>();
            
            float maxMP = targetProperty->getOriginal(BattleConfig::Property::MP);
            float curMP = targetProperty->get(BattleConfig::Property::MP);
            float mp = curMP + mpAdded;
            if (mp > maxMP) mp = maxMP;
            if (mp < 0) mp = 0;
            targetProperty->set(BattleConfig::Property::MP, mp);
            
            float actualMpChanged = mp - curMP;
            
            _EFFECT_CREATOR.createMPChangeEffect(*pFromEntity, *pTargetEntity, actualMpChanged, true, false);
        }
    }
    while (false);
    
    remove();
}
