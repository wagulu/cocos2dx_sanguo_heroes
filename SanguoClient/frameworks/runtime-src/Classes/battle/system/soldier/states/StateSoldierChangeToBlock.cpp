//
//  StateSoldierChangeToBlock.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#include "StateSoldierChangeToBlock.h"
#include "BattleWorld.h"

StateSoldierChangeToBlock::StateSoldierChangeToBlock(const BattleEvent::StrategySoldierChangeToBlock& eventBlock) : m_blockData(eventBlock)
{
    
}

StateSoldierChangeToBlock::~StateSoldierChangeToBlock()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void StateSoldierChangeToBlock::enter(entityx::Entity& entity)
{
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
 
    m_reverse = (entity.component<BattleComponent::Identify>()->side != BattleConfig::SIDE_LEFT);
    
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setVisible(false);
    
    auto position = entity.component<BattleComponent::Position>();
    
    if (m_blockData.appearEffect != nullptr)
    {
        m_appearEffectEntity = _ENTITY_MANAGER.create();
        
        BattleEvent::AddEffect event(m_blockData.appearEffect, m_reverse, position->x, position->y, position->z, 0, 0);
        event.effectEntity = m_appearEffectEntity;
        _ENTITY_EVENT.emit(event);
    }
    
    if (m_blockData.loopEffect != nullptr)
    {
        m_loopEffectEntity = _ENTITY_MANAGER.create();
        
        BattleEvent::AddEffect event(m_blockData.loopEffect, m_reverse, position->x, position->y, position->z, 0.8, -1);
        event.effectEntity = m_loopEffectEntity;
        _ENTITY_EVENT.emit(event);
    }
}

void StateSoldierChangeToBlock::exit(entityx::Entity& entity)
{
    if (m_appearEffectEntity.valid())
    {
        m_appearEffectEntity.destroy();
    }
    if (m_loopEffectEntity.valid())
    {
        m_loopEffectEntity.destroy();
    }
    
    if (m_blockData.deadEffect != nullptr)
    {
        auto position = entity.component<BattleComponent::Position>();
        
        _ENTITY_EVENT.emit<BattleEvent::AddEffect>(m_blockData.deadEffect, m_reverse, position->x, position->y, position->z, 0, 0);
    }
}

int StateSoldierChangeToBlock::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    if (m_blockData.lifeTime >= 0)
    {
        m_blockData.lifeTime -= dt;
        if (m_blockData.lifeTime <= 0)
        {
            _RETURN_STATE_RESULT(DEAD);
        }
    }
    
    return m_result;
}

void StateSoldierChangeToBlock::receive(const BattleEvent::HurtByAttack& e)
{
    if (m_blockData.entity.valid() && e.targetEntity == m_blockData.entity && m_loopEffectEntity.valid())
    {
        if (m_loopEffectEntity.has_component<BattleComponent::AnimationGroup>())
        {
            auto position = m_blockData.entity.component<BattleComponent::Position>();
            
            auto animation = m_loopEffectEntity.component<BattleComponent::AnimationGroup>();
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::G, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(0.1, 1.0f, 0.0f), 0);
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::G, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(0.1, 0.0f, 1.0f), 0.2f);
            
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::B, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(0.1, 1.0f, 0.0f), 0);
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::B, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(0.1, 0.0f, 1.0f), 0.2f);
            
            auto identifyT = e.fromEntity.component<BattleComponent::Identify>();
            auto dirX = identifyT->faceTo == BattleConfig::FACE_TO_RIGHT ? 16: -16;
            
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::X, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(0.05, position->x, position->x + dirX), 0);
            animation->m_tweenQueue.pushTween(TweenQueue::TARGET::X, TweenQueue::OPERATION::IN,
                                              new SimpleTween_linear(0.05, position->x + dirX, position->x), 0.05f);
        }
    }
}
