//
//  AnimationSystem.cpp
//  Game
//
//  Created by fuchenhao on 3/16/15.
//
//

#include "AnimationSystem.h"
#include "BattleWorld.h"
#include "../../entityx/entityx.h"


void AnimationSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::BattlePause>(*this);
    events.subscribe<BattleEvent::BattleResume>(*this);
    events.subscribe<BattleEvent::AddEntityAnimation>(*this);
    events.subscribe<BattleEvent::AddBuffAnimation>(*this);
    events.subscribe<BattleEvent::RemoveEntityAnimation>(*this);
}

void AnimationSystem::receive(const BattleEvent::BattlePause& e)
{
    BattleComponent::ActiveEntity::Handle active;
    
    for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(active))
    {
        active.remove();
    }
    
    m_isPausing = true;
}

void AnimationSystem::receive(const BattleEvent::BattleResume& e)
{
    m_isPausing = false;
}

void AnimationSystem::receive(const BattleEvent::AddEntityAnimation& e)
{
    auto entity = e.entity;
    if (entity.valid())
    {
        auto oldComp = entity.component<BattleComponent::EntityAnimation>();
        if (oldComp.valid()) oldComp.remove();
        
        auto position = entity.component<BattleComponent::Position>();
        auto animations = _BATTLE_ANIMATION_CREATOR.create(e.group, position->x, position->y, position->z, e.reverse);
        entity.assign<BattleComponent::EntityAnimation>(animations, e.loop, e.lifeTime, e.pTag);
    }
}

void AnimationSystem::receive(const BattleEvent::RemoveEntityAnimation& e)
{
    auto entity = e.entity;
    
    if (entity.has_component<BattleComponent::EntityAnimation>())
    {
        auto oldComp = entity.component<BattleComponent::EntityAnimation>();
        if (e.pTag == nullptr || *e.pTag == oldComp->tag) oldComp.remove();
    }
}

void AnimationSystem::receive(const BattleEvent::AddBuffAnimation& e)
{
    auto entity = e.entity;
    if (entity.valid())
    {
        auto oldComp = entity.component<BattleComponent::BuffAnimation>();
        if (oldComp.valid()) oldComp.remove();
        
        auto position = entity.component<BattleComponent::Position>();
        auto animations = _BATTLE_ANIMATION_CREATOR.create(e.group, position->x, position->y, position->z, e.reverse);
        entity.assign<BattleComponent::BuffAnimation>(animations, e.loop, e.lifeTime);
    }
}

void AnimationSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::Identify::Handle identify;
    BattleComponent::Animation::Handle animation;
    BattleComponent::AnimationGroup::Handle animationGroup;
    BattleComponent::ActiveEntity::Handle active;
    BattleComponent::Position::Handle position;
    BattleComponent::EntityAnimation::Handle entityAnimation;
    BattleComponent::BuffAnimation::Handle buffAnimation;
    
    for (entityx::Entity entity : es.entities_with_components(animation, identify))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            animation->battleAnimation->update(dt);
            animation->battleAnimation->setScaleX(identify->faceTo == BattleConfig::FACE_TO_RIGHT ? -CHARACTER_SCALE : CHARACTER_SCALE);

            std::vector<TweenQueueResult>& results = animation->m_tweenQueue.update(dt);
            for(TweenQueueResult& r : results)
            {
                applyTweenOperation(r.m_lastTarget, r.m_lastValue, animation->battleAnimation);
            }
        }
    }
    
    for (entityx::Entity entity : es.entities_with_components(animationGroup))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            animationGroup->battleAnimation->update(dt);
            
            std::vector<TweenQueueResult>& results = animationGroup->m_tweenQueue.update(dt);
            for(TweenQueueResult& r : results)
            {
                std::vector<BattleAnimation*> *pGroupAnims = animationGroup->battleAnimation->getAnimations();
                for(BattleAnimation* pEachAnim : *pGroupAnims)
                {
                    applyTweenOperation(r.m_lastTarget, r.m_lastValue, pEachAnim);
                }
            }
        }
    }
    
    for (entityx::Entity entity : es.entities_with_components(position, entityAnimation))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            entityAnimation->battleAnimation->update(dt);
            
            float deltaX = 0;
            float deltaY = 0;
            float deltaZ = 0;
            std::vector<TweenQueueResult>& results = entityAnimation->m_tweenQueue.update(dt);
            for(TweenQueueResult& r : results)
            {
                std::vector<BattleAnimation*> *pGroupAnims = entityAnimation->battleAnimation->getAnimations();
                
                if (r.m_lastTarget == TweenQueue::TARGET::X)
                {
                    deltaX = r.m_lastValue;
                }
                else if(r.m_lastTarget == TweenQueue::TARGET::Y)
                {
                    deltaY = r.m_lastValue;
                }
                else if(r.m_lastTarget == TweenQueue::TARGET::Z)
                {
                    deltaZ = r.m_lastValue;
                }
                
                for(BattleAnimation* pEachAnim : *pGroupAnims)
                {
                    applyTweenOperation(r.m_lastTarget, r.m_lastValue, pEachAnim);
                }
            }
            
            entityAnimation->battleAnimation->setPosition(position->x + deltaX, position->y + deltaY, position->z + deltaX);
            
            if (entityAnimation->lifeTime >= 0)
            {
                entityAnimation->lifeTime -= dt;
                if (entityAnimation->lifeTime <= 0)
                {
                    entityAnimation.remove();
                    break;
                }
            }
            if (!entityAnimation->loop && entityAnimation->battleAnimation->isComplete())
            {
                entityAnimation.remove();
                break;
            }
        }
    }
    
    for (entityx::Entity entity : es.entities_with_components(position, buffAnimation))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            buffAnimation->battleAnimation->update(dt);
            buffAnimation->battleAnimation->setPosition(position->x, position->y, position->z);
            
            if (buffAnimation->lifeTime >= 0)
            {
                buffAnimation->lifeTime -= dt;
                if (buffAnimation->lifeTime <= 0)
                {
                    buffAnimation.remove();
                    break;
                }
            }
            
            if (!buffAnimation->loop && buffAnimation->battleAnimation->isComplete())
            {
                buffAnimation.remove();
                break;
            }
        }
    }
}

void AnimationSystem::applyTweenOperation(TweenQueue::TARGET target, float value, BattleAnimation* anim)
{
    switch (target) {
        case TweenQueue::TARGET::ALPHA:
            anim->setAlpha(value);
            break;
        case TweenQueue::TARGET::R:
            anim->setColorR(value);
            break;
        case TweenQueue::TARGET::G:
            anim->setColorG(value);
            break;
        case TweenQueue::TARGET::B:
            anim->setColorB(value);
            break;
        case TweenQueue::TARGET::X:
            anim->setPositionX(value);
            break;
        case TweenQueue::TARGET::Y:
            anim->setPositionY(value);
            break;
        case TweenQueue::TARGET::Z:
            anim->setPositionZ(value);
            break;
        default:
            break;
    }
}

