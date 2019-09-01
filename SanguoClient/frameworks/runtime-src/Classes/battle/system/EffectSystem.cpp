//
//  EffectSystem.h
//  Game
//
//  Created by fuchenhao on 12/22/14.
//
//

#include "EffectSystem.h"

void EffectSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::AddEffect>(*this);
    events.subscribe<BattleEvent::AddEffectSimple>(*this);
    events.subscribe<BattleEvent::AddEffectToEntity>(*this);
    events.subscribe<BattleEvent::AddEffectFlyText>(*this);
    events.subscribe<BattleEvent::BattlePause>(*this);
    events.subscribe<BattleEvent::BattleResume>(*this);
}

void EffectSystem::receive(const BattleEvent::BattlePause& e)
{
    m_isPausing = true;
}

void EffectSystem::receive(const BattleEvent::BattleResume& e)
{
    m_isPausing = false;
}

void EffectSystem::receive(const BattleEvent::AddEffectToEntity& e)
{
    auto targetEnity = e.entity;
    auto position = targetEnity.component<BattleComponent::Position>();
    auto animations = _BATTLE_ANIMATION_CREATOR.create(e.group, position->x, position->y, position->z, e.reverse);
    
    auto entity = e.effectEntity;
    if (!entity.valid())
    {
        entity = _ENTITY_MANAGER.create();
    }
    
    entity.assign<BattleComponent::ActiveEntity>();
    
    auto animation = entity.assign<BattleComponent::AnimationGroup>(animations, false);
    animation->battleAnimation->setVisible(false);
    
    auto effect = entity.assign<BattleComponent::Effect>(e.delay, e.duration, true);
    effect->followTarget = e.entity;
    effect->fromId = e.fromId;
}

void EffectSystem::receive(const BattleEvent::AddEffect& e)
{
    auto animations = _BATTLE_ANIMATION_CREATOR.create(e.group, e.x, e.y, e.z, e.reverse, e.rotate);
    
    auto entity = e.effectEntity;
    if (!entity.valid())
    {
        entity = _ENTITY_MANAGER.create();
    }
    
    entity.assign<BattleComponent::ActiveEntity>();
    
    auto animation = entity.assign<BattleComponent::AnimationGroup>(animations);
    animation->battleAnimation->setVisible(false);
    
    auto effect = entity.assign<BattleComponent::Effect>(e.delay, e.duration);
    effect->fromId = e.fromId;
}

void EffectSystem::receive(const BattleEvent::AddEffectSimple& e)
{
    auto entity = _ENTITY_MANAGER.create();
    entity.assign<BattleComponent::ActiveEntity>();
    entity.assign<BattleComponent::AnimationGroup>(e.animations);
    entity.assign<BattleComponent::Effect>(e.delay, e.duration);
}

void EffectSystem::receive(const BattleEvent::AddEffectFlyText& e)
{
    auto& targetEntity = e.targetEntity;
    if (!targetEntity.valid())
    {
        return;
    }
    
    if (e.fireNow)
    {
        auto position = targetEntity.component<BattleComponent::Position>();
        _ENTITY_EVENT.emit<BattleEvent::AddEffectSimple>(e.animations, position->x, position->y, position->z);
        return;
    }
    
    if (!targetEntity.has_component<BattleComponent::FlyTextEffect>())
    {
        targetEntity.assign<BattleComponent::FlyTextEffect>(0.15);
    }
    
    auto flyTextEffect = targetEntity.component<BattleComponent::FlyTextEffect>();
    // 超过10个飘字则不会按顺序出，而是立即出
    if (flyTextEffect->animations.size() > 10)
    {
        auto position = targetEntity.component<BattleComponent::Position>();
        _ENTITY_EVENT.emit<BattleEvent::AddEffectSimple>(e.animations, position->x, position->y, position->z);
    }
    else
    {
        flyTextEffect->animations.push_back(e.animations);
    }
}

void EffectSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::Effect::Handle effect;
    BattleComponent::AnimationGroup::Handle animation;
    
    for (entityx::Entity entity : es.entities_with_components(effect, animation))
    {
        if ((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            if (effect->delay > 0)
            {
                effect->delay -= dt;
            }
            
            if (effect->delay <= 0)
            {
                if (!effect->playing)
                {
                    effect->playing = true;
                    
                    animation->battleAnimation->setVisible(true);
                    
                    if (effect->duration == 0)
                    {
                        animation->battleAnimation->playCurrent(false);
                    }
                    else
                    {
                        animation->battleAnimation->playCurrent(true, animation->randomStart);
                    }
                }
                
                if (effect->follow)
                {
                    if (effect->followTarget.valid())
                    {
                        auto tPosition = effect->followTarget.component<BattleComponent::Position>();
                        animation->battleAnimation->setPosition(tPosition->x, tPosition->y, tPosition->z);
                    }
                    else
                    {
                        entity.destroy();
                        continue;
                    }
                }
                
                if (effect->duration > 0)
                {
                    effect->elapse += dt;
                    if (effect->elapse >= effect->duration)
                    {
                        entity.destroy();
                    }
                }
                else if (effect->duration == 0 && animation->battleAnimation->isComplete())
                {
                    entity.destroy();
                }
            }
        }
    }
    
    BattleComponent::FlyTextEffect::Handle flyTextEffect;
    BattleComponent::Position::Handle position;
    
    for (auto entity : es.entities_with_components(position, flyTextEffect))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            if (flyTextEffect->animations.size() == 0)
            {
                flyTextEffect.remove();
            }
            else
            {
                flyTextEffect->delay -= dt;
                if (flyTextEffect->delay <= 0)
                {
                    _ENTITY_EVENT.emit<BattleEvent::AddEffectSimple>(flyTextEffect->animations[0], position->x, position->y, position->z);
                    
                    flyTextEffect->animations.erase(flyTextEffect->animations.begin());
                    flyTextEffect->delay = flyTextEffect->interval;
                }
            }
        }
    }
}

