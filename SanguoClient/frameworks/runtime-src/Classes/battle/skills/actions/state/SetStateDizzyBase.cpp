//
//  SetStateDizzyBase.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/23/15.
//
//

#include "SetStateDizzyBase.h"
#include "BattleWorld.h"

void SetStateDizzyBase::setTargetDizzy(int targetId)
{
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity == nullptr) return;
    
    float dizzyTime = 0;
    if (m_skillActionData->hasMember("time"))
    {
        dizzyTime = caclFormulaResult(*m_skillActionData->config["time"]->s);
    }
    
    bool dizzyEnable = false;
    if(m_skillActionData->hasMember("enable"))
    {
        if(MapHelper::getEntityType(targetId) == BattleConfig::ENTITY_HERO)
        {
            dizzyEnable = caclFormulaEnable(*m_skillActionData->config["enable"]->s, targetId);
        }
        else
        {
            int targetHeroId = (MapHelper::getSide(targetId) == BattleConfig::SIDE_LEFT) ? LEFT_HERO : RIGHT_HERO;
            dizzyEnable = caclFormulaEnable(*m_skillActionData->config["enable"]->s, targetHeroId);
        }
    }
    else if(m_skillActionData->hasMember("ratio"))
    {
        float dizzyRatio = caclFormulaResult(*m_skillActionData->config["ratio"]->s);
        dizzyEnable = cocos2d::random<float>(0.0f, 1.0f) <= dizzyRatio;
    }
    
    if (dizzyEnable)
    {
        if (MapHelper::isHero(targetId))
        {
            if (m_skillActionData->hasMember("heroDizzyConfig"))
            {
                auto* pDizzyConfig = m_skillData->pConfig->dizzy->getConfig(*m_skillActionData->config["heroDizzyConfig"]->s);
                if (pDizzyConfig != nullptr)
                {
                    BattleConfig::HeroDizzyData dizzyData;
                    dizzyData.skillEntityId = m_entityId;
                    dizzyData.dizzyTime = dizzyTime;
                    
                    _ENTITY_EVENT.emit(BattleEvent::StrategyHeroDizzy(*pTargetEntity, dizzyData, *pDizzyConfig));
                }
            }
        }
        else
        {
            _ENTITY_EVENT.emit(BattleEvent::StrategySoldierDizzy(*pTargetEntity, dizzyTime));
        }
    }
}
