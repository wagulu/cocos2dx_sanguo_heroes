//
//  RemoveEntityAnimation.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/22/15.
//
//

#include "RemoveEntityAnimation.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void RemoveEntityAnimation::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    do
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
            if (!skillTargetResults.valid()) break;
            targets = skillTargetResults->targets;
        }
        
        string* pTagName = m_skillActionData->hasMember("tag") ? m_skillActionData->config["tag"]->s : nullptr;
        
        for (int targetId : targets)
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity == nullptr) continue;
            
            _ENTITY_EVENT.emit<BattleEvent::RemoveEntityAnimation>(*pTargetEntity, pTagName);
        }
    }
    while (false);
    
    remove();
}
