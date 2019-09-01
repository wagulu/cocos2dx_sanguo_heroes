//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierDizzy.h"

#include "../../AICommonHelper.h"
#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "BattleEvent.h"

StateSoldierDizzy::StateSoldierDizzy(float dizzyTime) : m_dizzyTime(dizzyTime)
{
    
}

StateSoldierDizzy::~StateSoldierDizzy()
{
    
}

void StateSoldierDizzy::enter(entityx::Entity& entity)
{
    m_elapse = 0;
    
    auto animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_DIZZY);
    
    m_effectEntity = _ENTITY_MANAGER.create();
    
    auto* pEffectGroup = _BATTLE_ANIMATION_CREATOR.convertAnimationToEffectGroup("battleEffect", "animation_xuanyun");
    
    BattleEvent::AddEffectToEntity event(pEffectGroup, false, entity, 0, m_dizzyTime);
    event.effectEntity = m_effectEntity;
    _ENTITY_EVENT.emit(event);
    
    CC_SAFE_DELETE(pEffectGroup);
}

void StateSoldierDizzy::exit(entityx::Entity& entity)
{
    if (m_effectEntity.valid())
    {
        m_effectEntity.destroy();
    }
}

int StateSoldierDizzy::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    m_elapse += dt;
    
    if(m_elapse >= m_dizzyTime)
    {
        m_result = OVER;
    }
    
    return m_result;
}
