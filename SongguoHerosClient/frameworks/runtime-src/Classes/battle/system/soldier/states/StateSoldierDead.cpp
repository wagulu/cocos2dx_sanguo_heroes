//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierDead.h"
#include "AICommonHelper.h"
#include "BattleComponent.h"
#include "BattleConfig.h"
#include "AudioSystem.h"
#include "math.h"

StateSoldierDead::StateSoldierDead(const BattleConfig::AttackData& attackData, bool atOnce, bool needRebirth, uint64_t deadCauseID, float delay)
{
    m_damagePosition = attackData.damagePosition;
    m_atOnce = atOnce;
    m_needRebirth = needRebirth;
    m_deadCauseID = deadCauseID;
    m_delayTime = delay;
}

StateSoldierDead::StateSoldierDead()
{
    m_atOnce = true;
}

StateSoldierDead::~StateSoldierDead()
{
}

void StateSoldierDead::enter(entityx::Entity& entity)
{
//    if (entity.has_component<BattleComponent::FlyTextEffect>())
//    {
//        m_subState = (STATE_WAITING_FLY_TEXT_COMPLETE);
//    }
//    else
//    {
//        m_subState = (STATE_WAITING_FALLDOWN);
//    }

    BattleComponent::ObjectStateMachine::Handle objectStateMachine = entity.component<BattleComponent::ObjectStateMachine>();
    objectStateMachine->isDead = true;

}

void StateSoldierDead::exit(entityx::Entity& entity)
{
}

int StateSoldierDead::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }

//    if (entity.has_component<BattleComponent::FlyTextEffect>())
//    {
//        _RETURN_STATE_RESULT(WAITING_FLY_TEXT_COMPLETE);
//    }
    
    auto animation = entity.component<BattleComponent::Animation>();
    
    if (m_subState == STATE_WAITING_FLY_TEXT_COMPLETE)
    {
        if (!entity.has_component<BattleComponent::FlyTextEffect>() || m_atOnce)
        {
//            BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();
            
            if (!m_atOnce)
            {
                ensureAnimation(animation->battleAnimation, BattleConfig::ANI_DEATH1, false);
                playSound(entity);
                
                BattleComponent::Direction::Handle direction = entity.component<BattleComponent::Direction>();
                BattleComponent::Position::Handle position = entity.component<BattleComponent::Position>();
                if(m_damagePosition.isZero())
                {
                    direction->y = _V_SPEED_;
                    float speed = _H_SPEED_ * (cocos2d::rand_0_1() + 0.5);
                    float angle = cocos2d::rand_0_1() * CC_DEGREES_TO_RADIANS(360);
                    direction->x = speed * cos(angle);
                    direction->z = speed * sin(angle);
                }
                else
                {
                    direction->y = m_damagePosition.speedV * (1 + cocos2d::rand_minus1_1() * 0.2);
                    float speed = m_damagePosition.speedH * (1 + cocos2d::rand_minus1_1() * 0.2);
                    cocos2d::Vec2 vec(position->x - m_damagePosition.x, position->z - m_damagePosition.z);
                    vec.normalize();
                    direction->x = speed * vec.x;
                    direction->z = speed * vec.y;
                }
                
                if(m_damagePosition.group)
                {
                    bool reserse = entity.component<BattleComponent::Identify>()->faceTo == BattleConfig::FACE_TO_RIGHT;
                    _ENTITY_EVENT.emit<BattleEvent::AddEntityAnimation>(m_damagePosition.group, entity, reserse);
                }
            }
            else
            {
                if(m_delayTime > 0)
                {
                    m_delayTime -= dt;
                    return m_result;
                }
            }
            
            if (m_needRebirth)
            {
                auto position = entity.component<BattleComponent::Position>();
                int tileX = MapHelper::getTileX(position->x);
                int tileZ = MapHelper::getTileZ(position->z);
                
                auto id = _BATTLE_ENTITY.getTarget(tileZ, tileX);
                BattleConfig::Side side = MapHelper::getSide(id);                
                _BATTLE_ENTITY.removeEntity(entity);
                _ENTITY_EVENT.emit<BattleEvent::BattleActorRebirth>(entity, tileZ, tileX, side, m_deadCauseID);
            }
            else
            {
                _BATTLE_ENTITY.removeEntity(entity);
            }

            m_subState = STATE_WAITING_FALLDOWN;
        }
    }
    else if (m_subState == STATE_WAITING_FALLDOWN)
    {
        BattleComponent::Position::Handle position = entity.component<BattleComponent::Position>();
        BattleComponent::Direction::Handle direction = entity.component<BattleComponent::Direction>();
        direction->y -= dt * m_damagePosition.gravity;
        position->x += dt * direction->x;
        position->y += dt * direction->y;
        position->z += dt * direction->z;
        animation->battleAnimation->setPosition(position->x, position->y, position->z);
        if((direction->y < 0 && position->y <= 0) || m_atOnce)
        {
            if (!m_atOnce)
            {
                ensureAnimation(animation->battleAnimation, BattleConfig::ANI_DEATH2, false);
//                playSound(entity);
            }
            m_subState = STATE_WAITING_DEAD_ANIM;
        }
    }
    else if(m_subState == STATE_WAITING_DEAD_ANIM)
    {
        if(animation->battleAnimation->isComplete() || m_atOnce)
        {
            entity.destroy();
            _RETURN_STATE_RESULT(DONE);
        }
        
    }
    
    return m_result;
}

void StateSoldierDead::playSound(entityx::Entity& entity)
{
    
    std::string eventName = AUDIOEVENTS_SOLDIER_DEAD;
    std::string triggerName = "soldier"; //getHeroSoldierAudioTriggerName(entity);
    BattleEvent::BattleSound event(eventName, triggerName, entity);
    _ENTITY_EVENT.emit(event);

}

