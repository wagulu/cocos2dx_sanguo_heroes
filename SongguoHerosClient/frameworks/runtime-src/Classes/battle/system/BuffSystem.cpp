//
//  BuffSystem.cpp
//  Game
//
//  Created by fuchenhao on 3/3/15.
//
//

#include "BuffSystem.h"
#include "BattleConfig.h"
#include "BattleWorld.h"

using namespace entityx;

BuffSystem::BuffSystem()
{
}

void BuffSystem::configure(EventManager& events)
{
    events.subscribe<BattleEvent::AddBuff>(*this);
    events.subscribe<BattleEvent::RemoveBuff>(*this);
    events.subscribe<BattleEvent::ClearAllBuff>(*this);
}

void BuffSystem::update(EntityManager &es, EventManager &events, double dt)
{
    BattleComponent::Buff::Handle buff;
    BattleComponent::Property::Handle property;
    
    for (entityx::Entity entity : es.entities_with_components(buff, property))
    {
        std::vector<BattleConfig::BuffData*>& buffList = buff->buffList;
        std::vector<BattleConfig::BuffData*> buffToRemove;
        
        for (BattleConfig::BuffData* pBuffData : buffList)
        {
            // buff一直持续到战斗结束
            if (pBuffData->lifeTime == -1)
            {
                continue;
            }
            
            pBuffData->lifeTime -= dt;
            if (pBuffData->lifeTime <= 0)
            {
                buffToRemove.push_back(pBuffData);
            }
        }
        
        for (BattleConfig::BuffData* pBuffData : buffToRemove)
        {
            removeBuff(pBuffData, buff, property);
        }
    }
}

void BuffSystem::receive(const BattleEvent::AddBuff& e)
{
    addBuff(e.buffData);
}

void BuffSystem::receive(const BattleEvent::RemoveBuff& e)
{
    removeBuff(e.targetId, e.targetProperty);
}

void BuffSystem::receive(const BattleEvent::ClearAllBuff& e)
{
    removeBuff(e.targetId, BattleConfig::ALL_PROPERTIES);
}

void BuffSystem::addBuff(const BattleConfig::BuffData& buffData)
{
    entityx::Entity* entity = _BATTLE_ENTITY.getEntity(buffData.targetId);
    if (entity == NULL)
    {
        return;
    }
    
    BattleComponent::Property::Handle property = entity->component<BattleComponent::Property>();
    
    if (property->isLocked(buffData.targetProperty))
    {
        CCLOG("already locked, can not add buff");
        return;
    }
    
    if (!entity->has_component<BattleComponent::Buff>())
    {
        entity->assign<BattleComponent::Buff>();
    }
    BattleComponent::Buff::Handle buff = entity->component<BattleComponent::Buff>();
    
    if (hasLockBuff(buffData.targetProperty, buff))
    {
        CCLOG("already locked by buff, can not add buff");
        return;
    }
    
    float originalValue = property->getOriginal(buffData.targetProperty);
    
    BattleConfig::BuffData* data = new BattleConfig::BuffData(buffData);
    data->changeFinal = buffData.changeAbsolute + buffData.changePercent * originalValue;
    buff->buffList.push_back(data);
    
    // change value
    float currentValue = property->get(buffData.targetProperty);
    float newValue = currentValue + data->changeFinal;
    property->set(data->targetProperty, newValue);
    // 根据边界检查后的value修正changeFinal的值，newValue可能会大于该属性允许的最大值或小于最小值
    newValue = property->get(data->targetProperty);
    data->changeFinal = newValue - currentValue;
    
    CCLOG("buff added target:%d property:%d lifeTime:%f current:%f new:%f", data->targetId, data->targetProperty, data->lifeTime, currentValue, newValue);
    
    // add lock
    if (data->lock)
    {
        property->setLock(data->targetProperty, true);
    }
    
    _ENTITY_EVENT.emit<BattleEvent::UpdateBuffIcon>();
}

void BuffSystem::removeBuff(int targetId, BattleConfig::Property targetProperty)
{
    entityx::Entity* entity = _BATTLE_ENTITY.getEntity(targetId);
    if (entity == NULL || !entity->has_component<BattleComponent::Buff>())
    {
        return;
    }
    
    BattleComponent::Buff::Handle buff = entity->component<BattleComponent::Buff>();
    BattleComponent::Property::Handle property = entity->component<BattleComponent::Property>();
    
    // copy bufflist
    std::vector<BattleConfig::BuffData*> buffToRemove;
    for (BattleConfig::BuffData* pBuffData : buff->buffList)
    {
        if (pBuffData->targetProperty == targetProperty || targetProperty == BattleConfig::ALL_PROPERTIES)
        {
            buffToRemove.push_back(pBuffData);
        }
    }
    
    for (BattleConfig::BuffData* pBuffData : buffToRemove)
    {
        removeBuff(pBuffData, buff, property);
    }
}

void BuffSystem::removeBuff(BattleConfig::BuffData* pBuffData, BattleComponent::Buff::Handle& buff, BattleComponent::Property::Handle& property)
{
    if (!buff.valid())
    {
        return;
    }
    
    std::vector<BattleConfig::BuffData*>& buffList = buff->buffList;
    
    for (auto iter = buffList.begin(); iter != buffList.end(); )
    {
        if(*iter != NULL && *iter == pBuffData)
        {
            // release lock
            if (pBuffData->lock)
            {
                property->setLock(pBuffData->targetProperty, false);
            }
            
            // reset value
            float currentValue = property->get(pBuffData->targetProperty);
            float newValue = currentValue - pBuffData->changeFinal;
            property->set(pBuffData->targetProperty, newValue);
            CCLOG("buff removed %d %d %f %f", pBuffData->targetId, pBuffData->targetProperty, currentValue, property->get(pBuffData->targetProperty));
            
            // destruct
            delete pBuffData;
            iter = buffList.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    // remove component when no buff exist
    if (buff->buffList.size() == 0)
    {
        buff.remove();
    }
    
    _ENTITY_EVENT.emit<BattleEvent::UpdateBuffIcon>();
}

bool BuffSystem::hasLockBuff(BattleConfig::Property targetProperty, BattleComponent::Buff::Handle& buff)
{
    std::vector<BattleConfig::BuffData*>& buffList = buff->buffList;
    for (BattleConfig::BuffData* pBuffData : buffList)
    {
        if (pBuffData->targetProperty == targetProperty && pBuffData->lock)
        {
            return true;
        }
    }
    return false;
}
