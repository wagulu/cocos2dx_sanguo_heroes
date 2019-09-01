//
//  AddBuffBase.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#include "AddBuffBase.h"
#include "BattleWorld.h"
#include "EnumParser.h"
#include "SkillManager.h"

using namespace std;

void AddBuffBase::addBuffToTarget(int targetId)
{
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity != nullptr)
    {
        BattleConfig::BuffData buffData(targetId,
                                        EnumParserPropertyName().getValue(*m_skillActionData->config["property"]->s),
                                        0,
                                        0,
                                        caclFormulaResult(*m_skillActionData->config["time"]->s),
                                        m_skillActionData->config["lock"]->b);
        
        if (m_skillActionData->hasMember("changeByAbsolute") && m_skillActionData->config["changeByAbsolute"]->b == false)
        {
            buffData.changePercent = caclFormulaResult(*m_skillActionData->config["value"]->s);
        }
        else
        {
            buffData.changeAbsolute = caclFormulaResult(*m_skillActionData->config["value"]->s);
        }
        
        if(m_skillActionData->hasMember("effectLoop"))
        {
            auto* effectGroup = SkillManager::getInstance()->getBuffEffect(*m_skillActionData->config["effectLoop"]->s);
            _ENTITY_EVENT.emit<BattleEvent::AddBuffAnimation>(effectGroup, (*pTargetEntity), false, true, buffData.lifeTime);
        }
        
        if(m_skillActionData->hasMember("effectOnce"))
        {
            auto* effectGroup = SkillManager::getInstance()->getBuffEffect(*m_skillActionData->config["effectOnce"]->s);
            auto position = pTargetEntity->component<BattleComponent::Position>();
            _ENTITY_EVENT.emit<BattleEvent::AddEffect>(effectGroup, false, position->x, position->y, position->z, 0, 0, m_fromId);

        }
        
        if(m_skillActionData->hasMember("buffIcon"))
        {
            buffData.buffIcon = (*m_skillActionData->config["buffIcon"]->s);
        }
        
        
        _ENTITY_EVENT.emit<BattleEvent::AddBuff>(buffData);
    }
}















