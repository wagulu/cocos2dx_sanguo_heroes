//
//  Skill.cpp
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//

#include "SkillObject.h"
#include "SkillManager.h"
#include "MapHelper.h"

SkillObject::SkillObject(BattleConfig::SkillData* skillData, int fromId, const entityx::Entity::Id& entityId) : ActiveObject()
{
    m_skillData = skillData;
    m_fromId = fromId;
    m_side = MapHelper::getSide(fromId);
    m_entityId = entityId;
}

SkillObject::~SkillObject()
{
    for(auto action : m_skillActions)
    {
        delete action;
    }
    m_skillActions.clear();
}

void SkillObject::addAction(SkillAction* pAction, BattleConfig::SkillActionData* pActionData)
{
    pAction->setData(m_fromId, m_side, m_entityId, m_skillData, pActionData);
    m_skillActions.push_back(pAction);
}

void SkillObject::triggerAction(const std::string& trigger)
{
    m_pendingTriggers.push_back(trigger);
}

void SkillObject::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (m_pendingTriggers.size() > 0)
    {
        for (auto& trigger : m_pendingTriggers)
        {
            for(auto* pActionData : m_skillData->pConfig->actions)
            {
                if (pActionData->trigger == trigger)
                {
                    auto* pAction = SkillManager::getInstance()->buildAction(pActionData->name);
                    if (pAction) addAction(pAction, pActionData);
                }
            }
            
            if(m_skillActions.size() > 0)
            {
                for(auto it = m_skillActions.begin(); it != m_skillActions.end(); it++)
                {
                    auto action = *it;
                    action->onTriggerFired(trigger);
                }
            }
        }
        m_pendingTriggers.clear();
        std::vector<std::string>().swap(m_pendingTriggers);
    }
    
    if (m_skillActions.size() > 0)
    {
        for (auto it = m_skillActions.begin(); it != m_skillActions.end();)
        {
            auto action = *it;
            if (action->shouldRemove())
            {
                std::string* pTriggerOnRemove = action->m_skillActionData->pTriggerOnRemove;
                if (pTriggerOnRemove != nullptr)
                {
                    m_removeTriggers.push_back(pTriggerOnRemove);
                }
                    
                CC_SAFE_DELETE(action);
                it = m_skillActions.erase(it);
                
                //m_skillActions.erase(it);
                //break;
            }
            else
            {
                if(action->hasFired())
                {
                    action->update(es, elapse, dt);
                    
                    //经过一段时间自动销毁
                    if(action->m_endTime >= 0 && action->m_endTime <= elapse)
                    {
                        action->remove();
                    }
                }
                else if(action->m_skillActionData->startTime <= elapse)
                {
                    if (action->m_skillActionData->lifeTime >= 0)
                    {
                        action->m_endTime = elapse + action->m_skillActionData->lifeTime;
                    }
                    else
                    {
                        action->m_endTime = -1;
                    }
                   
                    action->fireAction(es);
                }
                
                it++;
            }
        }
        
        if (m_removeTriggers.size() > 0)
        {
            for (auto* pRemoveTrigger : m_removeTriggers)
            {
                triggerAction(*pRemoveTrigger);
            }
            m_removeTriggers.clear();
            std::vector<std::string*>().swap(m_removeTriggers);
        }
    }
    else
    {
        remove();
    }
}
