//
//  GasSystem.cpp
//  Game
//
//  Created by zhaikaiyu on 3/2/15.
//
//

#include "GasSystem.h"
#include "AICommonHelper.h"
#include "BattleWorld.h"

#include "StateHeroIdle.h"
#include "StateHeroRun.h"
#include "StateHeroAttack.h"
#include "StateHeroDead.h"

void GasSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::GasGather::Handle gasGather;
    BattleComponent::Property::Handle property;
    BattleComponent::Identify::Handle identify;
    BattleComponent::ObjectStateMachine::Handle objectStateMachine;

    for (entityx::Entity entity : es.entities_with_components(gasGather, property, identify, objectStateMachine))
    {
        float currentMP = property->get(BattleConfig::Property::MP);
        
        for (auto* pData : gasGather->skills)
        {
            if(pData->pConfig->type == BattleConfig::SkillType::POSITIVE)
            {
                pData->changeEnergy(dt * pData->energySpeed);
                
                bool lockedByFiredSkill = false;
                
                auto battleType = _BATTLE_STAGE.getBattleType();
                if (battleType == BattleConfig::BattleType::PVE || battleType == BattleConfig::BattleType::DUEL)
                {
                    if(identify->side == BattleConfig::SIDE_LEFT)
                    {
                        lockedByFiredSkill = this->m_skillLockLeft;
                    }
                    else
                    {
                        lockedByFiredSkill = this->m_skillLockLeft || this->m_skillLockRight;
                    }
                }
                else
                {
                    //TO-DO PVP or others
                }
                
                bool isDizzy = objectStateMachine->getCurrentStateCode() == BattleConfig::GENERAL_DIZZY;
                
                pData->isReady = (pData->energy >= pData->energyCost && currentMP >= pData->mpCost && (!gasGather->lock) && (!lockedByFiredSkill));
                pData->isActive = (currentMP >= pData->mpCost && (!gasGather->lock) && (!lockedByFiredSkill) && (!isDizzy));
            }
        }
    }
}


void GasSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::ChangeGasEvent>(*this);
    events.subscribe<BattleEvent::PreFireSkill>(*this);
    events.subscribe<BattleEvent::LockSkill>(*this);
    events.subscribe<BattleEvent::UnlockSkill>(*this);
}

void GasSystem::receive(const BattleEvent::PreFireSkill& e)
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(e.fromId);
    if (pEntity == nullptr) return;
    if (isFreeToSendSkill(*pEntity))
    {
        auto gasGather = pEntity->component<BattleComponent::GasGather>();
        auto property = pEntity->component<BattleComponent::Property>();
        for (auto* pSkillData : gasGather->skills)
        {
            if (pSkillData->id == e.skillId && pSkillData->isReady)
            {
                pSkillData->changeEnergy(-pSkillData->energyCost);
                pSkillData->isReady = false;
                pSkillData->canBreak = true;
                
                float mp = property->get(BattleConfig::Property::MP) - pSkillData->mpCost;
                if (mp < 0) mp = 0;
                property->set(BattleConfig::Property::MP, mp);
                
                _ENTITY_EVENT.emit<BattleEvent::FireSkill>(pSkillData, e.fromId);
            }
        }
    }
}

void GasSystem::receive(const BattleEvent::ChangeGasEvent& e)
{
    if(!_ENTITY_MANAGER.valid(e.entityId)) return;
    auto hero = _ENTITY_MANAGER.get(e.entityId);
    auto general = hero.component<BattleComponent::General>();
 
    switch (e.type) {
        case BattleConfig::GAS_KILLSOLDIER://杀死士兵
        {
            recoverEnergy(hero, general->config.ERKillBase);//攻击敌方士兵回复
            break;
        }
        case BattleConfig::GAS_SOLDIERKILLED://自己失败被对方杀死
        {
            recoverEnergy(hero, general->config.ERLoseBase);//攻击敌方士兵回复
            break;
        }
        case BattleConfig::GAS_ATTACKBYSOLDIER://被士兵攻击
        {
            int maxHp = hero.component<BattleComponent::Property>()->getOriginal(BattleConfig::Property::HP);
            recoverEnergy(hero, general->config.ERHPBase * e.damage / maxHp);
            break;
        }
        default:
            break;
    }
}

void GasSystem::emitLockStatusChange()
{
    bool lockedByFiredSkill = false;
    
    auto battleType = _BATTLE_STAGE.getBattleType();
    if (battleType == BattleConfig::BattleType::PVE || battleType == BattleConfig::BattleType::DUEL)
    {
        lockedByFiredSkill = this->m_skillLockLeft;
    }
    else
    {
        //TO-DO PVP or others
    }
    
    if(lockedByFiredSkill)
    {
        BattleComponent::GasGather::Handle gasGather;
        BattleComponent::Identify::Handle identify;
        
        for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(gasGather, identify))
        {
            for (auto* pData : gasGather->skills)
            {
                if(pData->pConfig->type == BattleConfig::SkillType::POSITIVE)
                {
                    bool lockedByFiredSkill = false;
                    
                    auto battleType = _BATTLE_STAGE.getBattleType();
                    if (battleType == BattleConfig::BattleType::PVE || battleType == BattleConfig::BattleType::DUEL)
                    {
                        if(identify->side == BattleConfig::SIDE_LEFT)
                        {
                            lockedByFiredSkill = this->m_skillLockLeft;
                        }
                        else
                        {
                            lockedByFiredSkill = this->m_skillLockLeft || this->m_skillLockRight;
                        }
                    }
                    else
                    {
                        //TO-DO PVP or others
                    }
                    
                    pData->isReady = false;
                    pData->isActive = false;
                }
            }
        }
    }
    _ENTITY_EVENT.emit<BattleEvent::LockStatusChange>(lockedByFiredSkill);
}

void GasSystem::receive(const BattleEvent::LockSkill& e)
{
    if(e.side == BattleConfig::SIDE_LEFT)
    {
        this->m_skillLockLeft = true;
        
        emitLockStatusChange();
    }
    else
    {
        this->m_skillLockRight = true;
    }
}

void GasSystem::receive(const BattleEvent::UnlockSkill& e)
{
    if(e.side == BattleConfig::SIDE_LEFT)
    {
        this->m_skillLockLeft = false;
        
        emitLockStatusChange();
    }
    else
    {
        this->m_skillLockRight = false;
    }
}


void GasSystem::recoverEnergy(entityx::Entity& entity, float value)
{
    auto gasGather = entity.component<BattleComponent::GasGather>();
    for (auto* pData : gasGather->skills)
    {
        pData->changeEnergy(value);
    }
}
