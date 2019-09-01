//
//  StateSoldierFadeIn.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 4/17/15.
//
//

#include "StateSoldierFadeIn.h"

#include "../../AICommonHelper.h"
#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "BattleEvent.h"

StateSoldierFadeIn::StateSoldierFadeIn(float dizzyTime) : m_totalTime(dizzyTime)
{
    m_inTime = dizzyTime / 3 * 2;
    m_moveTween = new SimpleTween_linear(m_totalTime, 200.0f, 0.0f);
}

StateSoldierFadeIn::~StateSoldierFadeIn()
{
    
}

void StateSoldierFadeIn::enter(entityx::Entity& entity)
{
    m_elapse = 0;
    
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
    
    animation->battleAnimation->setVisible(false);
    animation->battleAnimation->setAlpha(0);
    
    animation->battleAnimation->setPositionY(0);
    
    animation->m_tweenQueue.clearTween();
    animation->m_tweenQueue.pushTween(TweenQueue::TARGET::ALPHA, TweenQueue::OPERATION::IN, new SimpleTween_linear(m_totalTime - m_inTime, 0.0f, 1.0f), m_inTime);
}

void StateSoldierFadeIn::exit(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setVisible(true);
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
    
    delete m_moveTween;
}

int StateSoldierFadeIn::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    if (m_elapse >= m_inTime)
    {
        auto animation = entity.component<BattleComponent::Animation>();
        animation->battleAnimation->setVisible(true);
        m_inTime = -1;
    }
    
    auto position = entity.component<BattleComponent::Position>();
    auto animation = entity.component<BattleComponent::Animation>();
    
    position->y = m_moveTween->out(dt);
    animation->battleAnimation->setPositionY(position->y);
    
    m_elapse += dt;
    
    if(m_elapse >= m_totalTime)
    {
        m_result = OVER;
    }
    
    return m_result;
}
