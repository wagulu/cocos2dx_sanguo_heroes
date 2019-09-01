//
//  ShowHeroEffect.cpp
//  Game
//
//  Created by fuchenhao on 3/3/15.
//
//

#include "ShowHeroEffect.h"
#include "BattleWorld.h"

void ShowHeroEffect::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    if(m_skillActionData->hasMember("delay"))
    {
        m_delay = m_skillActionData->config["delay"]->d;
    }
    else
    {
        fire();
    }
}

void ShowHeroEffect::update(entityx::EntityManager &es, float elapse, float dt)
{
    m_elapse += dt;
    if(m_elapse >= m_delay) fire();
}

void ShowHeroEffect::fire()
{
    
    int targetId = 0;
    
    BattleConfig::DamageSide side = (BattleConfig::DamageSide)m_skillActionData->config["side"]->d;
    switch (side)
    {
        case BattleConfig::DamageSide::SELF:
            targetId = m_fromId;
            break;
        case BattleConfig::DamageSide::ENEMY:
            //TODO, lrui, multi general
            targetId = m_fromId == LEFT_HERO ? RIGHT_HERO : LEFT_HERO;
            break;
            
        default:
            break;
    }
    
    int duration = 0;
    
    if (m_skillActionData->hasMember("duration"))
    {
        duration = m_skillActionData->config["duration"]->d;        //side info
    }
    else if (m_skillActionData->hasMember("durationFormula"))
    {
        duration = caclFormulaResult(*m_skillActionData->config["durationFormula"]->s);        //side info
    }
    else if(_ENTITY_MANAGER.get(m_entityId).has_component<BattleComponent::SkillDuration>())
    {
        duration = _ENTITY_MANAGER.get(m_entityId).component<BattleComponent::SkillDuration>()->duration;
    }
    
    float randomDeltaX = 0;
    float randomDeltaY = 0;
    if (m_skillActionData->hasMember("random"))
    {
        float dValue = m_skillActionData->config["random"]->d;
        randomDeltaX = cocos2d::random<float>(-dValue, dValue);
        randomDeltaY = cocos2d::random<float>(-dValue, dValue);
    }
    
    bool follow = false;
    if (m_skillActionData->hasMember("follow"))
    {
        follow = m_skillActionData->config["follow"]->d;        //side info
    }
    
    
    if (targetId > 0)
    {
        // play animation
        entityx::Entity* e = _BATTLE_ENTITY.getEntity(targetId);
        
        if(e)
        {
            auto position = e->component<BattleComponent::Position>();
            auto identify = e->component<BattleComponent::Identify>();
            
            BattleConfig::EffectGroup* group = m_effect->getEffect(*(m_skillActionData->config["animation"]->s));
            bool reserse = identify->faceTo == BattleConfig::FACE_TO_RIGHT;
            
            if(m_skillActionData->hasMember("reverse") && m_skillActionData->config["reverse"]->b)
            {
                reserse = !reserse;
            }
            
            if (group)
            {
                if (follow)
                {
                    _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(group, reserse, *e, 0, duration, m_fromId);
                }
                else
                {
                    bool rotate = false;
                    if(m_skillActionData->hasMember("rotate")) rotate = m_skillActionData->config["rotate"]->b;
                    _ENTITY_EVENT.emit<BattleEvent::AddEffect>(group, reserse, position->x + randomDeltaX, position->y + randomDeltaY, position->z, 0, duration, m_fromId, rotate);
                }
            }
        }
    }
    
    remove();
};
