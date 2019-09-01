//
//  DizzyActionSkillGuiJiShiHun.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/4/15.
//
//

#include "DizzyActionSkillGuiJiShiHun.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

DizzyActionSkillGuiJiShiHun::~DizzyActionSkillGuiJiShiHun()
{
    if (m_moveFastTween != nullptr)
    {
        CC_SAFE_DELETE(m_moveFastTween);
    }
    if (m_knockBackTween != nullptr)
    {
        CC_SAFE_DELETE(m_knockBackTween);
    }
}

void DizzyActionSkillGuiJiShiHun::enter(entityx::Entity& entity)
{
    m_config.absorbFromId = (int)m_pActionConfig->config["absorbFromId"].d;
    m_config.absorbSlowSpeed = (float)m_pActionConfig->config["absorbSlowSpeed"].d;
    m_config.absorbFastTile = (float)m_pActionConfig->config["absorbFastTile"].d;
    m_config.absorbFastTime = (float)m_pActionConfig->config["absorbFastTime"].d;
    m_config.knockBackTime = (float)m_pActionConfig->config["knockBackTime"].d;
    m_config.knockBackValidDistance = (int)m_pActionConfig->config["knockBackValidDistance"].d;
    m_config.knockBackDuration = (float)m_pActionConfig->config["knockBackDuration"].d;
    m_config.knockBackTile = (int)m_pActionConfig->config["knockBackTile"].d;
    m_config.knockBackDizzyDuration = (float)m_pActionConfig->config["knockBackDizzyDuration"].d;
}

void DizzyActionSkillGuiJiShiHun::exit(entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setBiasZ(0);
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE, true);
    
    _HERO_MOVEMENT.moveTo(position->x, position->y, position->z, entity, HeroMovement::IGNORE_ALL_SOLDIER);
}

bool DizzyActionSkillGuiJiShiHun::isComplete()
{
    return m_subState == SUB_STATE::DONE;
}

void DizzyActionSkillGuiJiShiHun::update(entityx::Entity& entity, float dt)
{
    if (m_subState == SUB_STATE::DONE)
    {
        return;
    }
    
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(m_config.absorbFromId);
    if (pTargetEntity == nullptr)
    {
        m_subState = SUB_STATE::DONE;
        return;
    }
    
    auto direction = entity.component<BattleComponent::Direction>();
    auto position = entity.component<BattleComponent::Position>();
    auto animation = entity.component<BattleComponent::Animation>();
    auto targetPos = pTargetEntity->component<BattleComponent::Position>();
    
    if (m_config.knockBackTime > 0) m_config.knockBackTime -= dt;
    if (m_config.absorbFastTime > 0) m_config.absorbFastTime -= dt;
    
    if (m_subState == SUB_STATE::ABSORB_SLOW)
    {
        if (m_config.absorbFastTime <= 0)
        {
            animation->battleAnimation->setBiasZ(2);
            animation->battleAnimation->setPositionZ(position->z);
            animation->battleAnimation->pauseCurrent(true);
            
            m_moveFastToX = position->x + (position->x < targetPos->x ? 1 : -1) * m_config.absorbFastTile * GRID_WIDTH;
            if (position->x < targetPos->x && m_moveFastToX > targetPos->x) m_moveFastToX = targetPos->x;
            if (position->x > targetPos->x && m_moveFastToX < targetPos->x) m_moveFastToX = targetPos->x;
            m_moveFastTween = new SimpleTween_quad(m_config.knockBackTime - m_config.absorbFastTime, position->x, m_moveFastToX);
            
            m_subState = SUB_STATE::ABSORB_FAST;
        }
        else
        {
            ensureAnimation(animation->battleAnimation, BattleConfig::ANI_HIT);
            
            float moveToX = targetPos->x + (position->x < targetPos->x ? -2 : 2) * GRID_WIDTH;
            
            _SIMPLE_MOVEMENT.updateDirection(direction, position, moveToX, position->y, position->z);
            
            _HERO_MOVEMENT.move(dt, moveToX, position->y, position->z, entity, HeroMovement::IGNORE_ALL_SOLDIER, m_config.absorbSlowSpeed);
        }
    }
    else if (m_subState == SUB_STATE::ABSORB_FAST)
    {
        if (m_config.knockBackTime <= 0)
        {
            animation->battleAnimation->pauseCurrent(false);
            animation->battleAnimation->setBiasZ(0);
            animation->battleAnimation->setPositionZ(position->z);
            
            float currentX = m_moveFastTween->current();
            
            if (abs(MapHelper::getTileX(currentX) - MapHelper::getTileX(targetPos->x)) <= m_config.knockBackValidDistance)
            {
                m_knockBackToX = currentX + (position->x < targetPos->x ? -1 : 1) * m_config.knockBackTile * GRID_WIDTH;
                m_knockBackTween = new SimpleTween_cubic(m_config.knockBackDuration, currentX, m_knockBackToX);
                
                m_subState = SUB_STATE::KNOCK_BACK;
            }
            else
            {
                _HERO_MOVEMENT.moveTo(currentX, position->y, position->z, entity, HeroMovement::IGNORE_ALL_SOLDIER);
                m_config.knockBackDizzyDuration += m_config.knockBackDuration;
                
                m_subState = SUB_STATE::KNOCK_BACK_DIZZY;
            }
        }
        else
        {
            float moveToX = m_moveFastTween->out(dt);
            animation->battleAnimation->setPositionX(moveToX);
        }
    }
    else if (m_subState == SUB_STATE::KNOCK_BACK)
    {
        float moveToX = m_knockBackTween->out(dt);
        animation->battleAnimation->setPositionX(moveToX);
        
        if (moveToX == m_knockBackToX)
        {
            _HERO_MOVEMENT.moveTo(m_knockBackToX, position->y, position->z, entity, HeroMovement::IGNORE_ALL_SOLDIER);
            
            m_subState = SUB_STATE::KNOCK_BACK_DIZZY;
        }
    }
    else if (m_subState == SUB_STATE::KNOCK_BACK_DIZZY)
    {
        m_config.knockBackDizzyDuration -= dt;
        if (m_config.knockBackDizzyDuration <= 0)
        {
            m_subState = SUB_STATE::DONE;
        }
    }
}
