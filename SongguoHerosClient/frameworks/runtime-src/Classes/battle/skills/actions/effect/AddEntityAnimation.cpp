//
//  AddEntityAnimation.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/16/15.
//
//

#include "AddEntityAnimation.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AddEntityAnimation::fireAction(entityx::EntityManager &es)
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
            duration = caclFormulaResult(*m_skillActionData->config["duration"]->s);
        }
        else
        {
            auto skillDuration = es.get(m_entityId).component<BattleComponent::SkillDuration>();
            if (!skillDuration.valid()) break;
            duration = skillDuration->duration;
        }
        
        bool loop = m_skillActionData->config["loop"]->b;
        string* pHeroAniName = m_skillActionData->hasMember("heroAni") ? m_skillActionData->config["heroAni"]->s : nullptr;
        string* pSoldierAniName = m_skillActionData->hasMember("soldierAni") ? m_skillActionData->config["soldierAni"]->s : nullptr;
        string* pTagName = m_skillActionData->hasMember("tag") ? m_skillActionData->config["tag"]->s : nullptr;
        
        for (int targetId : targets)
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity == nullptr) continue;
            
            auto identify = pTargetEntity->component<BattleComponent::Identify>();
            bool reserse = identify->faceTo == BattleConfig::FACE_TO_RIGHT;
            
            if (identify->type == BattleConfig::EntityType::ENTITY_HERO)
            {
                if (pHeroAniName != nullptr)
                {
                    auto* pEffectGroup = m_effect->getEffect(*pHeroAniName);
                    _ENTITY_EVENT.emit<BattleEvent::AddEntityAnimation>(pEffectGroup, *pTargetEntity, reserse, loop, duration, pTagName);
                }
            }
            else
            {
                if (pSoldierAniName != nullptr)
                {
                    auto* pEffectGroup = m_effect->getEffect(*pSoldierAniName);
                    _ENTITY_EVENT.emit<BattleEvent::AddEntityAnimation>(pEffectGroup, *pTargetEntity, reserse, loop, duration, pTagName);
                }
            }
        }
    }
    while (false);
    
    remove();
}
