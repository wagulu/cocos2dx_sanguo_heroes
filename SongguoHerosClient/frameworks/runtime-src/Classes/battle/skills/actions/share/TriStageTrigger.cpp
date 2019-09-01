//
//  TriStageTrigger.cpp
//  sanguoGame
//
//  Created by fuchenhao on 8/17/15.
//
//

#include "TriStageTrigger.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void TriStageTrigger::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    float duration = 0;
    if(m_skillActionData->hasMember("duration"))
    {
        duration = m_skillActionData->config["duration"]->d;
    }
    else
    {
        duration = this->caclFormulaResult(*m_skillActionData->config["durationFormula"]->s);
    }
    
    float phase1 = m_skillActionData->config["phase1"]->d;
    float phase3 = m_skillActionData->config["phase3"]->d;
    float phase2 = duration - phase1 - phase3;
    if(phase2 < 0)
    {
        remove();
        return;
    }
    
    m_stage2Duration = phase2;
    
    m_stageTime[0] = 0;
    m_stageTime[1] = phase1;
    m_stageTime[2] = duration - phase3 - 0.1;
    
    m_stageFired[0] = false;
    m_stageFired[1] = false;
    m_stageFired[2] = false;
    
    addAnimation(1);
}


void TriStageTrigger::update(entityx::EntityManager &es, float elapse, float dt)
{
    m_elapse += dt;
    
    for(int i = 0; i < 3; i++)
    {
        if((!m_stageFired[i]) && m_elapse >= m_stageTime[i])
        {
            addAnimation(i + 1);
        }
    }
    
    if(m_stageFired[0] && m_stageFired[1] && m_stageFired[2])
    {
        remove();
    }
}

void TriStageTrigger::addAnimation(int stage)
{
    m_stageFired[stage - 1] = true;
    
    std::string animationName = (*m_skillActionData->config["animation" + Value(stage).asString()]->s);

    float loopTime = (stage == 2) ? m_stage2Duration : 0.0;
    
    TargetFinder::TargetResults targetResults;
    vector<int>& targets = targetResults.targets;
    auto skillTarget = EnumParserSkillTarget().getValue(*m_skillActionData->config["SkillTarget"]->s);
    _TARGET_FINDER.findSkillTargets(m_fromId, skillTarget, targetResults);
    
    for (int targetId : targets)
    {
        auto* targetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if(targetEntity)
        {
            auto* effectGroup = this->m_effect->getEffect(animationName);
            auto position = targetEntity->component<BattleComponent::Position>();
            bool reverse = (targetEntity->component<BattleComponent::Identify>()->side == BattleConfig::SIDE_LEFT) ? true : false;
            _ENTITY_EVENT.emit<BattleEvent::AddEffect>(effectGroup, reverse, position->x, position->y, position->z, 0, loopTime, m_fromId);
        }
    }
}




























