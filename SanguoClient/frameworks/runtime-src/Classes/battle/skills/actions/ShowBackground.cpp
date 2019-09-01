//
//  ShowBackground.cpp
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//

#include "ShowBackground.h"
#include "BattleWorld.h"

void ShowBackground::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    if (m_skillActionData->hasMember("showDuration"))
    {
        m_showDuration = m_skillActionData->config["showDuration"]->d;
    }
    if (m_skillActionData->hasMember("showColor"))
    {
        m_showColor = m_skillActionData->config["showColor"]->d;
    }
    if (m_skillActionData->hasMember("hideDuration"))
    {
        m_hideDuration = m_skillActionData->config["hideDuration"]->d;
    }
    if (m_skillActionData->hasMember("hideDelay"))
    {
        m_hideDelay = m_skillActionData->config["hideDelay"]->d;
    }
    if (m_skillActionData->hasMember("hideTrigger"))
    {
        m_pHideTrigger = m_skillActionData->config["hideTrigger"]->s;
    }
    
    _BATTLE_STAGE.showBackgroundMask(m_showDuration, m_showColor);
    
    m_hideRequired = true;
}

void ShowBackground::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (m_hideDelay > 0)
    {
        m_hideDelay -= dt;
        if (m_hideDelay <= 0)
        {
            hideBackground();
            remove();
        }
    }
}

void ShowBackground::onTriggerFired(const std::string& trigger)
{
    if (m_pHideTrigger && *m_pHideTrigger == trigger)
    {
        hideBackground();
        remove();
    }
}

void ShowBackground::hideBackground()
{
    if (m_hideRequired)
    {
        _BATTLE_STAGE.hideBackgroundMask(m_hideDuration);
        m_hideRequired = false;
    }
}

ShowBackground::~ShowBackground()
{
    hideBackground();
}


