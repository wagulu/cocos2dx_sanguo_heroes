//
//  DizzyActionSkillChiTianBaFangYan.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#include "DizzyActionSkillChiTianBaFangYan.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "SkillObject.h"

DizzyActionSkillChiTianBaFangYan::~DizzyActionSkillChiTianBaFangYan()
{
    clearMoveTween();
    
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByGhost>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByGhost>(*this);
    }
}

void DizzyActionSkillChiTianBaFangYan::enter(entityx::Entity& entity)
{
    m_flyData.flyUpTime = (float)m_pActionConfig->config["flyUpTime"].d;
    m_flyData.flyUpHeight = (float)m_pActionConfig->config["flyUpHeight"].d;
    m_flyData.dropDownDelay = (float)m_pActionConfig->config["dropDownDelay"].d;
    m_flyData.dropDownTime = (float)m_pActionConfig->config["dropDownTime"].d;
    m_flyData.knockBackTime = (float)m_pActionConfig->config["knockBackTime"].d;
    m_flyData.knockBackTile = (int)m_pActionConfig->config["knockBackTile"].d;
    m_flyData.knockBackDizzyTime = (float)m_pActionConfig->config["knockBackDizzyTime"].d;
    m_flyData.lastHitSoldierId = (int)m_pActionConfig->config["lastHitSoldierId"].d;
    m_flyData.firstHitSoldierId = (int)m_pActionConfig->config["firstHitSoldierId"].d;
    m_flyData.pLastHitTrigger = m_pActionConfig->config["lastHitTrigger"].s;
    m_flyData.pHeroDropTrigger = m_pActionConfig->config["heroDropTrigger"].s;
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByGhost>(*this);
    
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setColor(0.7, 0.7, 1.0, 1.0);
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_HIT, false);
    
    auto position = entity.component<BattleComponent::Position>();
    position->y = 0;
    
    m_moveTween = new SimpleTween_quad(m_flyData.flyUpTime, 0.0f, m_flyData.flyUpHeight);
}

void DizzyActionSkillChiTianBaFangYan::exit(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setColor(1.0, 1.0, 1.0, 1.0);
    animation->battleAnimation->setPositionY(0);
    
    auto position = entity.component<BattleComponent::Position>();
    position->y = 0;
}

void DizzyActionSkillChiTianBaFangYan::update(entityx::Entity& entity, float dt)
{
    if (m_subState == FlyState::DONE)
    {
        return;
    }
    
    if (m_subState == FlyState::FLY_UP)
    {
        auto position = entity.component<BattleComponent::Position>();
        auto animation = entity.component<BattleComponent::Animation>();
        
        position->y = m_moveTween->out(dt);
        animation->battleAnimation->setPositionY(position->y);
        
        if (m_moveTween->isFinish())
        {
            m_subState = FlyState::FLY_TOP;
            m_elapse = 0;
        }
    }
    else if (m_subState == FlyState::FLY_TOP)
    {
        m_elapse += dt;
        if (m_flyData.lastHitSoldierId == 0 && m_elapse >= m_flyData.dropDownDelay)
        {
            m_subState = FlyState::DROP_DOWN;
            
            clearMoveTween();
            auto position = entity.component<BattleComponent::Position>();
            m_moveTween = new SimpleTween_quad(m_flyData.dropDownTime, position->y, 0.0f);
        }
    }
    else if (m_subState == FlyState::DROP_DOWN)
    {
        auto position = entity.component<BattleComponent::Position>();
        position->y = m_moveTween->in(dt);
        
        auto animation = entity.component<BattleComponent::Animation>();
        animation->battleAnimation->setPositionY(position->y);
        
        if (m_moveTween->isFinish())
        {
            fireSkillTrigger(m_flyData.pHeroDropTrigger);
            
            m_subState = FlyState::DONE;
        }
    }
    else if (m_subState == FlyState::KNOCK_DOWN)
    {
        auto position = entity.component<BattleComponent::Position>();
        position->y = 0;
        
        auto animation = entity.component<BattleComponent::Animation>();
        animation->battleAnimation->setPositionY(0);
        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_DIZZY, true);
        
        m_subState = FlyState::KNOCK_DOWN_COMPLETE;
    }
    else if (m_subState == FlyState::KNOCK_BACK_PREPARE)
    {
        auto animation = entity.component<BattleComponent::Animation>();
        ensureAnimation(animation->battleAnimation, BattleConfig::ANI_DIZZY, true);
        
        auto identify = entity.component<BattleComponent::Identify>();
        auto position = entity.component<BattleComponent::Position>();
        int tileXBias = (identify->faceTo == BattleConfig::FaceTo::FACE_TO_RIGHT ? -1 : 1) * m_flyData.knockBackTile;
        
        clearMoveTween();
        m_moveTween = new SimpleTween_quad(m_flyData.knockBackTime, 0.0f, 1.0f);
        m_beginPos.set(position->x, position->y, position->z);
        m_endPos.set(position->x + tileXBias * GRID_WIDTH, 0, position->z);
        m_deltaPos.set(m_endPos.x - m_beginPos.x, m_endPos.y - m_beginPos.y, 0);
        
        m_subState = FlyState::KNOCK_BACK;
    }
    else if (m_subState == FlyState::KNOCK_BACK)
    {
        float ratio = m_moveTween->out(dt);
        float x = m_beginPos.x + m_deltaPos.x * ratio;
        float y = m_beginPos.y + m_deltaPos.y * ratio;
        float z = m_beginPos.z;
        
        auto animation = entity.component<BattleComponent::Animation>();
        animation->battleAnimation->setPosition(x, y, z);
        
        if (m_moveTween->isFinish())
        {
            _HERO_MOVEMENT.moveTo(x, y, z, entity, HeroMovement::IGNORE_ALL_SOLDIER);
            
            m_subState = FlyState::KNOCK_BACK_DIZZY;
            m_elapse = 0;
        }
    }
    else if (m_subState == FlyState::KNOCK_BACK_DIZZY)
    {
        m_elapse += dt;
        if (m_elapse >= m_flyData.knockBackDizzyTime)
        {
            m_subState = FlyState::DONE;
        }
    }
}

bool DizzyActionSkillChiTianBaFangYan::isComplete()
{
    return m_subState == FlyState::DONE;
}

void DizzyActionSkillChiTianBaFangYan::clearMoveTween()
{
    if (m_moveTween != nullptr)
    {
        CC_SAFE_DELETE(m_moveTween);
    }
}

void DizzyActionSkillChiTianBaFangYan::fireSkillTrigger(std::string* pTrigger)
{
    if (pTrigger && _ENTITY_MANAGER.valid(m_pDizzyData->skillEntityId))
    {
        auto skillEntity = _ENTITY_MANAGER.get(m_pDizzyData->skillEntityId);
        auto skillComp = skillEntity.component<BattleComponent::Skill>();
        
        if (skillComp->skill)
        {
            skillComp->skill->triggerAction(*pTrigger);
        }
    }
}

void DizzyActionSkillChiTianBaFangYan::receive(const BattleEvent::HurtByGhost& e)
{
    if (e.ghostSoldierId == m_flyData.lastHitSoldierId)
    {
        m_subState = FlyState::KNOCK_BACK_PREPARE;
        
        fireSkillTrigger(m_flyData.pLastHitTrigger);
        fireSkillTrigger(m_flyData.pHeroDropTrigger);
    }
    else if (e.ghostSoldierId == m_flyData.firstHitSoldierId)
    {
        m_subState = FlyState::KNOCK_DOWN;
    }
}
