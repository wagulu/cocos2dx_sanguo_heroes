//
//  StateSoldierChangeToGhost.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/8/15.
//
//

#include "StateSoldierChangeToGhost.h"
#include "AICommonHelper.h"

using namespace cocos2d;

StateSoldierChangeToGhost::StateSoldierChangeToGhost(const GhostData& ghostData) : m_ghostData(ghostData)
{
}

StateSoldierChangeToGhost::~StateSoldierChangeToGhost()
{
    clearMoveTween();
}

void StateSoldierChangeToGhost::enter(entityx::Entity& entity)
{
    auto* pTargetEntity = _BATTLE_ENTITY.getEntityEvenIfDead(m_ghostData.targetId);
    if (pTargetEntity != nullptr)
    {
        m_targetEntity = *pTargetEntity;
        
        prepareToFlyUp(entity);
    }
    else
    {
        m_result = ENEMY_NOT_EXIST;
    }
}

void StateSoldierChangeToGhost::exit(entityx::Entity& entity)
{
}

bool StateSoldierChangeToGhost::isDelegateState()
{
    return (m_result == WORKING);
}

bool StateSoldierChangeToGhost::isDelegateStateWorking()
{
    return isDelegateState();
}

int StateSoldierChangeToGhost::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    if (!m_targetEntity.valid())
    {
        _RETURN_STATE_RESULT(ENEMY_NOT_EXIST);
    }
    
    if (m_subState == SUB_STATE::FLY_UP)
    {
        moveSoldierFlyingUp(entity, dt);
        
        if (m_moveTween->isFinish())
        {
            prepareToChangeToGhost(entity);
            
            m_subState = SUB_STATE::CHANGE_TO_GHOST;
        }
    }
    else if (m_subState == SUB_STATE::CHANGE_TO_GHOST)
    {
        auto animationGroup = entity.component<BattleComponent::AnimationGroup>();
        if (animationGroup->battleAnimation->isComplete())
        {
            prepareToMoveAwayEnemy(entity);
            
            m_subState = SUB_STATE::MOVE_AWAY_ENEMY;
        }
    }
    else if (m_subState == SUB_STATE::MOVE_AWAY_ENEMY)
    {
        moveGhostAwayFromEnemy(entity, dt);
        
        if (m_moveTween->isFinish())
        {
            prepareToMoveCloseEnemy(entity);
            
            m_subState = SUB_STATE::MOVE_CLOSE_ENEMY;
        }
    }
    else if (m_subState == SUB_STATE::MOVE_CLOSE_ENEMY)
    {
        moveGhostCloseToEnemy(entity, dt);
        
        if (m_moveTween->isFinish())
        {
            doAttack(entity);
            
            _RETURN_STATE_RESULT(DONE);
        }
    }
    
    return m_result;
}

void StateSoldierChangeToGhost::clearMoveTween()
{
    if (m_moveTween != nullptr) CC_SAFE_DELETE(m_moveTween);
}

void StateSoldierChangeToGhost::prepareToFlyUp(entityx::Entity& entity)
{
    auto targetPos = m_targetEntity.component<BattleComponent::Position>();
    auto targetIdentify = m_targetEntity.component<BattleComponent::Identify>();
    
    auto myPos = entity.component<BattleComponent::Position>();
    auto myAnimation = entity.component<BattleComponent::Animation>();
    auto myIdentify = entity.component<BattleComponent::Identify>();
    
    float startX = myPos->x;
    
    if (m_ghostData.isLastHit)
    {
        myIdentify->faceTo = (targetIdentify->faceTo == BattleConfig::FaceTo::FACE_TO_LEFT ? BattleConfig::FaceTo::FACE_TO_RIGHT : BattleConfig::FaceTo::FACE_TO_LEFT);
        
        float deltaX = myPos->x - targetPos->x;
        if (deltaX > 0 && targetIdentify->faceTo == BattleConfig::FaceTo::FACE_TO_LEFT) startX = targetPos->x - deltaX;
        if (deltaX < 0 && targetIdentify->faceTo == BattleConfig::FaceTo::FACE_TO_RIGHT) startX = targetPos->x - deltaX;
    }
    else
    {
        myIdentify->faceTo = (myPos->x > targetPos->x ? BattleConfig::FaceTo::FACE_TO_LEFT : BattleConfig::FaceTo::FACE_TO_RIGHT);
    }
    
    ensureAnimation(myAnimation->battleAnimation, BattleConfig::ANI_DEATH1, true);
    
    m_moveTween = new SimpleTween_quint(m_ghostData.flyUpTime, m_ghostData.flyUpStartSpeed, m_ghostData.flyUpEndSpeed);
    m_endPos.set(startX, myPos->y, myPos->z);
    m_deltaPos.set(Vec3(targetPos->x, targetPos->y, targetPos->z), Vec3(startX, myPos->y + m_ghostData.flyUpHeight, myPos->z));
    m_deltaPos.normalize();
    m_deltaPos.x *= m_ghostData.flyUpFactorX;
    
    m_scale = CHARACTER_SCALE + (myPos->z < targetPos->z ? min(m_ghostData.scaleFactor * abs(myPos->z - targetPos->z), m_ghostData.scaleMax) : 0);
}

void StateSoldierChangeToGhost::prepareToChangeToGhost(entityx::Entity& entity)
{
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setVisible(false);
    
    if (entity.has_component<BattleComponent::AnimationGroup>())
    {
        entity.component<BattleComponent::AnimationGroup>().remove();
    }
    auto *pAnimation = _BATTLE_ANIMATION_CREATOR.create(m_ghostData.appearEffect, m_endPos.x, m_endPos.y, m_endPos.z);
    auto animationGroup = entity.assign<BattleComponent::AnimationGroup>(pAnimation);
    animationGroup->battleAnimation->playCurrent(false);
    animationGroup->battleAnimation->setScale(m_scale);
}

void StateSoldierChangeToGhost::prepareToMoveAwayEnemy(entityx::Entity& entity)
{
    if (entity.has_component<BattleComponent::AnimationGroup>())
    {
        entity.component<BattleComponent::AnimationGroup>().remove();
    }
    auto *pAnimation = _BATTLE_ANIMATION_CREATOR.create(m_ghostData.loopEffect, m_endPos.x, m_endPos.y, m_endPos.z);
    auto animationGroup = entity.assign<BattleComponent::AnimationGroup>(pAnimation);
    animationGroup->battleAnimation->playCurrent(true);
    animationGroup->battleAnimation->setScale(m_scale);
    
    auto targetPos = m_targetEntity.component<BattleComponent::Position>();
    
    clearMoveTween();
    m_moveTween = new SimpleTween_quad(m_ghostData.moveAwayTime, 0.0f, 1.0f);
    m_beginPos.set(targetPos->x, 0, targetPos->z);
    m_deltaPos.set(m_beginPos, m_endPos);
    m_deltaPos.normalize();
    m_deltaPos.scale(m_ghostData.moveAwayDistance);
    m_beginPos.set(m_endPos);
    m_endPos.set(m_beginPos.x + m_deltaPos.x, m_beginPos.y + m_deltaPos.y, m_beginPos.z + m_deltaPos.z);
}

void StateSoldierChangeToGhost::prepareToMoveCloseEnemy(entityx::Entity& entity)
{
    auto targetPos = m_targetEntity.component<BattleComponent::Position>();
    
    float randomOffsetX = (cocos2d::random() % 2 == 0 ? -1 : 1) * m_ghostData.hitOffset;
    float randomOffsetY = (cocos2d::random() % 2 == 0 ? 0 : 1) * m_ghostData.hitOffset;
    
    clearMoveTween();
    m_moveTween = new SimpleTween_quad(m_ghostData.moveCloseTime, 0.0f, 1.0f);
    m_beginPos.set(m_endPos);
    m_endPos.set(targetPos->x + randomOffsetX, targetPos->y + randomOffsetY, targetPos->z);
    m_deltaPos.set(m_endPos.x - m_beginPos.x, m_endPos.y - m_beginPos.y, m_endPos.z - m_beginPos.z);
}

void StateSoldierChangeToGhost::moveSoldierFlyingUp(entityx::Entity& entity, float dt)
{
    float speed = m_moveTween->out(dt);
    float x = m_endPos.x + dt * speed * m_deltaPos.x;
    float y = m_endPos.y + dt * speed * m_deltaPos.y;
    float z = m_endPos.z + dt * speed * m_deltaPos.z;
    m_endPos.set(x, y, z);
    
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setPosition(x, y, z);
}

void StateSoldierChangeToGhost::moveGhostAwayFromEnemy(entityx::Entity& entity, float dt)
{
    float ratio = m_moveTween->out(dt);
    float x = m_beginPos.x + m_deltaPos.x * ratio;
    float y = m_beginPos.y + m_deltaPos.y * ratio;
    float z = m_beginPos.z + m_deltaPos.z * ratio;
    
    auto animationGroup = entity.component<BattleComponent::AnimationGroup>();
    animationGroup->battleAnimation->setPosition(x, y, z);
}

void StateSoldierChangeToGhost::moveGhostCloseToEnemy(entityx::Entity& entity, float dt)
{
    float ratio = m_moveTween->in(dt);
    float x = m_beginPos.x + m_deltaPos.x * ratio;
    float y = m_beginPos.y + m_deltaPos.y * ratio;
    float z = m_beginPos.z + m_deltaPos.z * ratio;
    
    auto animationGroup = entity.component<BattleComponent::AnimationGroup>();
    animationGroup->battleAnimation->setPosition(x, y, z);
}

void StateSoldierChangeToGhost::doAttack(entityx::Entity& entity)
{
    if (entity.has_component<BattleComponent::AnimationGroup>())
    {
        entity.component<BattleComponent::AnimationGroup>().remove();
    }
    _ENTITY_EVENT.emit<BattleEvent::AddEffect>(m_ghostData.hitEffect, false, m_endPos.x, m_endPos.y, m_endPos.z, 0, 0);
    
    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(m_ghostData.attackData);
    
    _ENTITY_EVENT.emit<BattleEvent::HurtByGhost>(entity.component<BattleComponent::Identify>()->id, m_ghostData.attackData.getTargetId());
}
