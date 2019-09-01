//
//  ColorTween.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/10/15.
//
//

#include "ColorTween.h"
#include "BattleWorld.h"

void ColorTween::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
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
    
    auto* pEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pEntity != nullptr)
    {
        float duration = m_skillActionData->config["duration"]->d;
        
        auto animation = pEntity->component<BattleComponent::Animation>();
        
        if(m_skillActionData->hasMember("r"))
        {
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::R, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(duration, animation->battleAnimation->getColorR(), m_skillActionData->config["r"]->d), 0);
        }
        
        if(m_skillActionData->hasMember("g"))
        {
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::G, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(duration, animation->battleAnimation->getColorG(), m_skillActionData->config["g"]->d), 0);
        }
        
        if(m_skillActionData->hasMember("b"))
        {
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::B, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(duration, animation->battleAnimation->getColorB(), m_skillActionData->config["b"]->d), 0);
        }
        
        if(m_skillActionData->hasMember("a"))
        {
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::ALPHA, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(duration, animation->battleAnimation->getAlpha(), m_skillActionData->config["a"]->d), 0);
        }
    }
    
    remove();
};
