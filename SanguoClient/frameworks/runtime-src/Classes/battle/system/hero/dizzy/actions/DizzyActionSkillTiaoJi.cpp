//
//  DizzyActionSkillTiaoJi.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/16/15.
//
//

#include "DizzyActionSkillTiaoJi.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "SkillObject.h"

DizzyActionSkillTiaoJi::~DizzyActionSkillTiaoJi()
{
    clearMoveTween();
}

void DizzyActionSkillTiaoJi::enter(entityx::Entity& entity)
{
    m_flyHeight = (float)m_pActionConfig->config["flyUpHeight"].d;
    // check direction
    auto identify = entity.component<BattleComponent::Identify>();
    m_flyDistance = (float)m_pActionConfig->config["flyDistance"].d;
    m_flyDistance *= (identify->side == BattleConfig::SIDE_LEFT) ? -1 : 1;
    m_flyUpTime = (float)m_pActionConfig->config["flyUpTime"].d;
    m_delay = (float)m_pActionConfig->config["delay"].d;
    m_dropTime = (float)m_pActionConfig->config["dropTime"].d;
    m_targetX = (float)m_pActionConfig->config["targetX"].d;
    
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_HIT, false);
    
    auto position = entity.component<BattleComponent::Position>();
    position->y = 0;
    
    m_moveTweenY = new SimpleTween_quad(m_flyUpTime, 0.0f, m_flyHeight);
    m_moveTweenX = new SimpleTween_quad(m_flyUpTime, position->x, m_targetX);
}

void DizzyActionSkillTiaoJi::exit(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setPositionY(0);
    
    auto position = entity.component<BattleComponent::Position>();
    position->y = 0;
}

void DizzyActionSkillTiaoJi::update(entityx::Entity& entity, float dt)
{
    if (m_subState == FlyState::DONE)
    {
        return;
    }

    if (m_subState == FlyState::FLY_UP)
    {
        auto position = entity.component<BattleComponent::Position>();
        _HERO_MOVEMENT.moveTo(m_moveTweenX->out(dt), m_moveTweenY->out(dt), position->z, entity, HeroMovement::MoveIgnoreType::IGNORE_ALL_SOLDIER);
        
        if (m_moveTweenY->isFinish())
        {
            m_subState = FlyState::FLY_TOP;
            m_elapse = 0;
        }
    }
    else if (m_subState == FlyState::FLY_TOP)
    {
        m_elapse += dt;
        if (m_elapse >= m_delay)
        {
            m_subState = FlyState::DROP_DOWN;
            
            clearMoveTween();
            auto position = entity.component<BattleComponent::Position>();
            m_moveTweenY = new SimpleTween_quad(m_dropTime, position->y, 0.0f);
            m_moveTweenX = new SimpleTween_quad(m_dropTime, position->x, position->x + m_flyDistance);
        }
    }
    else if (m_subState == FlyState::DROP_DOWN)
    {
        auto position = entity.component<BattleComponent::Position>();
        _HERO_MOVEMENT.moveTo(m_moveTweenX->out(dt), m_moveTweenY->out(dt), position->z, entity, HeroMovement::MoveIgnoreType::IGNORE_ALL_SOLDIER);
        
        if (m_moveTweenY->isFinish())
        {
            m_subState = FlyState::DONE;
        }
    }
}

bool DizzyActionSkillTiaoJi::isComplete()
{
    return m_subState == FlyState::DONE;
}

void DizzyActionSkillTiaoJi::clearMoveTween()
{
    if (m_moveTweenY != nullptr)
    {
        CC_SAFE_DELETE(m_moveTweenY);
    }
    if (m_moveTweenX != nullptr)
    {
        CC_SAFE_DELETE(m_moveTweenX);
    }
}
