//
//  AddEntityEffect.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/18/15.
//
//

#include "AddEntityEffect.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AddEntityEffect::fireAction(entityx::EntityManager &es)
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
        
        float duration = 0;
        if (m_skillActionData->hasMember("duration"))
        {
            duration = m_skillActionData->config["duration"]->d;
        }
        else if (m_skillActionData->hasMember("durationFormula"))
        {
            duration = caclFormulaResult(*m_skillActionData->config["durationFormula"]->s);
        }
        else
        {
            auto skillDuration = es.get(m_entityId).component<BattleComponent::SkillDuration>();
            if (!skillDuration.valid()) break;
            duration = skillDuration->duration;
        }
        
        bool follow = (m_skillActionData->hasMember("follow") ? m_skillActionData->config["follow"]->b : true);
        
        auto* pEffectGroup = m_effect->getEffect(*m_skillActionData->config["effect"]->s);
        if (pEffectGroup == nullptr) break;
       
        for (int targetId : targets)
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity == nullptr) continue;
            
            auto identify = pTargetEntity->component<BattleComponent::Identify>();
            bool reserse = identify->faceTo == BattleConfig::FACE_TO_RIGHT;
            
            if (follow)
            {
                _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(pEffectGroup, reserse, *pTargetEntity, 0, duration, m_fromId);
            }
            else
            {
                auto position = pTargetEntity->component<BattleComponent::Position>();
                
                _ENTITY_EVENT.emit<BattleEvent::AddEffect>(pEffectGroup, reserse, position->x, position->y, position->z, 0, duration, m_fromId);
            }
            
        }
    }
    while (false);
    
    remove();
}
