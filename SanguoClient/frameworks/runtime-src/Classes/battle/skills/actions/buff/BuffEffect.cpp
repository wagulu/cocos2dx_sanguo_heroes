//
//  BuffEffect.cpp
//  sanguoGame
//
//  Created by fuchenhao on 8/7/15.
//
//

#include "BuffEffect.h"
#include "BattleWorld.h"
#include "EnumParser.h"
#include "SkillManager.h"

using namespace std;

void BuffEffect::fireAction(entityx::EntityManager &es)
{
    
    if (m_skillActionData->hasMember("SkillTarget"))
    {
        TargetFinder::TargetResults targetResults;
        vector<int>& targets = targetResults.targets;
        auto skillTarget = EnumParserSkillTarget().getValue(*m_skillActionData->config["SkillTarget"]->s);
        _TARGET_FINDER.findSkillTargets(m_fromId, skillTarget, targetResults);
        
        float time = 0;
        if(m_skillActionData->hasMember("time"))
        {
            time = m_skillActionData->config["time"]->d;
        }
        else if (m_skillActionData->hasMember("timeFormula"))
        {
            time = caclFormulaResult(*m_skillActionData->config["timeFormula"]->s);
        }
        
        if(time >0)
        {
            for (int targetId : targets)
            {
                
                auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
                if (pTargetEntity != nullptr)
                {
                    if(m_skillActionData->hasMember("effectLoop"))
                    {
                        auto* effectGroup = SkillManager::getInstance()->getBuffEffect(*m_skillActionData->config["effectLoop"]->s);
                        _ENTITY_EVENT.emit<BattleEvent::AddBuffAnimation>(effectGroup, (*pTargetEntity), false, true, time);
                    }
                    
                    if(m_skillActionData->hasMember("effectOnce"))
                    {
                        auto* effectGroup = SkillManager::getInstance()->getBuffEffect(*m_skillActionData->config["effectOnce"]->s);
                        auto position = pTargetEntity->component<BattleComponent::Position>();
                        _ENTITY_EVENT.emit<BattleEvent::AddEffect>(effectGroup, false, position->x, position->y, position->z, 0, 0, m_fromId);
                        
                    }
                }
            }
        }
    }
        
    remove();
}

