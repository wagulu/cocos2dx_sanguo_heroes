//
//  ArrowSystem.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/7/15.
//
//

#include "ArrowSystem.h"


void ArrowSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::AddSkillArrow>(*this);
    events.subscribe<BattleEvent::BattlePause>(*this);
    events.subscribe<BattleEvent::BattleResume>(*this);
}

void ArrowSystem::receive(const BattleEvent::AddSkillArrow& e)
{
    bool reverse = e.data.face == BattleConfig::FACE_TO_RIGHT;
    float speed = (e.data.side == BattleConfig::SIDE_LEFT) ? e.data.speed : -e.data.speed;
    
    auto entity = _ENTITY_MANAGER.create();
    entity.assign<BattleComponent::ActiveEntity>();
    entity.assign<BattleComponent::Position>(e.x, 0, e.z-1);
    entity.assign<BattleComponent::SkillArrow>(e.data, MapHelper::getTileX(e.x), MapHelper::getTileZ(e.z), e.destroyAfterHitHero);
    
    if(e.data.type == BattleConfig::ARROW_SKILL_VERTICAL)
    {
        entity.assign<BattleComponent::Direction>(0, 0, -speed);
    }
    else if(e.data.type == BattleConfig::ARROW_SKILL_LINE_TRACK)
    {
        int targetId = (e.data.side == BattleConfig::SIDE_LEFT) ? RIGHT_HERO : LEFT_HERO;
        
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if (pTargetEntity != nullptr)
        {
            auto targetPosition = pTargetEntity->component<BattleComponent::Position>();
            auto selfPosition = entity.component<BattleComponent::Position>();
            
            //set direction
            float targetX, targetZ;
            if(abs(targetPosition->x - selfPosition->x) <= e.data.breakDistance)
            {
                //fly to enemy
                entity.component<BattleComponent::SkillArrow>()->track = BattleComponent::ARROW_TRACKING;
                selfPosition->z = e.data.targetZ;
                targetX = targetPosition->x;
                targetZ = targetPosition->z;
            }
            else
            {
                //fly to target z
                targetX = targetPosition->x - ((e.data.side == BattleConfig::SIDE_LEFT) ? e.data.breakDistance : -e.data.breakDistance);
                targetZ = e.data.targetZ;
            }
            
            cocos2d::Vec2 vec(targetX - selfPosition->x, targetZ - selfPosition->z);
            vec.normalize();
            entity.assign<BattleComponent::Direction>(e.data.speed * vec.x, 0, e.data.speed * vec.y);
        }
        else
        {
            entity.destroy();
            return;
        }
    }
    else
    {
        entity.assign<BattleComponent::Direction>(speed, 0, 0);
    }
    _ENTITY_EVENT.emit<BattleEvent::AddEntityAnimation>(e.group, entity, reverse);
    
    if(e.follow)
    {
        _ENTITY_EVENT.emit<BattleEvent::CameraFollowEntity>(entity);
    }
}

void ArrowSystem::receive(const BattleEvent::BattlePause& e)
{
    m_isPausing = true;
}

void ArrowSystem::receive(const BattleEvent::BattleResume& e)
{
    m_isPausing = false;
}

void ArrowSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::SkillArrow::Handle arrow;
    BattleComponent::Position::Handle position;
    BattleComponent::Direction::Handle diretion;
    
    for (entityx::Entity entity : es.entities_with_components(arrow, position, diretion))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            BattleConfig::MoveArrowResult result;
            
            switch (arrow->arrowData.type)
            {
                case BattleConfig::ARROW_SKILL_LINE:
                {
                    result = _ARROW_MOVEMENT.moveLine(dt, entity);
                    break;
                }
                
                case BattleConfig::ARROW_SKILL_VERTICAL:
                {
                    result = _ARROW_MOVEMENT.moveVertical(dt, entity);
                    break;
                }
                    
                case BattleConfig::ARROW_SKILL_CURVE:
                {
                    result = _ARROW_MOVEMENT.moveCurve(dt, entity);
                    break;
                }
                    
                case BattleConfig::ARROW_SKILL_CIRCLE:
                {
                    break;
                }
                    
                case BattleConfig::ARROW_SKILL_LINE_TRACK:
                {
                    if(arrow->track == BattleComponent::ARROW_FLYING)
                    {
                        result = _ARROW_MOVEMENT.moveLine(dt, entity);
                        
                        //check whether change to track mode
                        int targetId = (arrow->arrowData.side == BattleConfig::SIDE_LEFT) ? RIGHT_HERO : LEFT_HERO;
                        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
                        if (pTargetEntity != nullptr)
                        {
                            auto targetPosition = pTargetEntity->component<BattleComponent::Position>();
                            auto selfPosition = entity.component<BattleComponent::Position>();
                            
                            if(abs(targetPosition->x - selfPosition->x) <= arrow->arrowData.breakDistance)
                            {
                                arrow->track = BattleComponent::ARROW_TRACKING;
                            }
                        }
                    }
                    else if(arrow->track == BattleComponent::ARROW_TRACKING)
                    {
                        _ARROW_MOVEMENT.updateDirection(dt, entity);
                        result = _ARROW_MOVEMENT.moveLine(dt, entity);
                    }
                    break;
                }
                    
                default:
                    break;
            }
            
            if(result == BattleConfig::ARROW_OUT)
            {
                auto position = entity.component<BattleComponent::Position>();
                _ENTITY_EVENT.emit<BattleEvent::SkillArrowOutEvent>(arrow->arrowData.skillId, arrow->arrowData.id, position->x, position->z);
                _ENTITY_EVENT.emit<BattleEvent::SkillArrowDestroyEvent>(arrow->arrowData.skillId, arrow->arrowData.id);
                entity.destroy();
            }
            else if (result==BattleConfig::ARROW_HIT)
            {
                _ENTITY_EVENT.emit<BattleEvent::SkillArrowHitEvent>(arrow->arrowData.skillId, arrow->arrowData.id, arrow->targets, entity);
                if(arrow->hitHero && arrow->destroyAfterHitHero)
                {
                    if(arrow->arrowData.trigger != "")
                    {
                        _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(arrow->arrowData.id, arrow->arrowData.trigger);
                    }
                    _ENTITY_EVENT.emit<BattleEvent::SkillArrowDestroyEvent>(arrow->arrowData.skillId, arrow->arrowData.id);
                    entity.destroy();
                }
                if(arrow->arrowData.hp > 0)
                {
                    for(auto target : arrow->targets)
                    {
                        auto targetEntity = _BATTLE_ENTITY.getEntity(target);
                        if(targetEntity)
                        {
                            auto property = targetEntity->component<BattleComponent::Property>();
                            auto hp = property->get(BattleConfig::Property::HP);
                            arrow->arrowData.hp -= hp;
                        }
                    }
                    if(arrow->arrowData.hp <= 0)
                    {
                        _ENTITY_EVENT.emit<BattleEvent::SkillArrowOutOfHpEvent>(arrow->arrowData.skillId, arrow->arrowData.id, position->x, position->z);
                        _ENTITY_EVENT.emit<BattleEvent::SkillArrowDestroyEvent>(arrow->arrowData.skillId, arrow->arrowData.id);
                        entity.destroy();
                    }
                }
            }
            else if (result==BattleConfig::ARROW_NO_HIT)
            {
                
            }
        }
    }
}
