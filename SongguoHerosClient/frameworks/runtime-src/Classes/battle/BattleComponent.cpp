//
//  BattleComponent.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 7/1/15.
//
//

#include "BattleComponent.h"
#include "AudioManager.h"
#include "AudioSystem.h"
#include "SkillObject.h"

BattleComponent::Property::Property(const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& data,
                                    const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& dataMin,
                                    const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& dataMax) : m_min(dataMin), m_max(dataMax)
{
    m_lock.fill(false);
    m_current.fill(0);
    m_original.fill(0);
    
    for (int i = 0; i < data.size(); i++)
    {
        set((BattleConfig::Property)i, data[i]);
    }
    m_original = m_current;
}

float BattleComponent::Property::get(BattleConfig::Property name)
{
    return m_current[name];
}

void BattleComponent::Property::set(BattleConfig::Property name, float value)
{
    if (m_lock[name]) return;
    
    float minValue = m_min[name];
    float maxValue = m_max[name];
    if (value < minValue) value = minValue;
    else if (value > maxValue) value = maxValue;
    
    m_current[name] = value;
}

float BattleComponent::Property::getOriginal(BattleConfig::Property name)
{
    return m_original[name];
}

bool BattleComponent::Property::isLocked(BattleConfig::Property name)
{
    return m_lock[name];
}

void BattleComponent::Property::setLock(BattleConfig::Property name, bool lock)
{
    CCLOG("set property lock %d %d", name, lock);
    m_lock[name] = lock;
}




void BattleComponent::AI::clearCompletedAI()
{
    for (auto iter = ai.begin(); iter != ai.end(); )
    {
        auto* pData = *iter;
        if (pData == nullptr || pData->state == BattleConfig::AI_COMPLETE)
        {
            if (pData != nullptr) CC_SAFE_DELETE(pData);
            iter = ai.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}



BattleComponent::FlyTextEffect::~FlyTextEffect()
{
    for(auto pVector : animations)
    {
        if (pVector == nullptr) continue;
        for (auto pAni : *pVector)
        {
            if (pAni == nullptr) continue;
            delete pAni;
            pAni = nullptr;
        }
        pVector->clear();
        delete pVector;
        pVector = nullptr;
    }
    animations.clear();
}



BattleComponent::Skill::~Skill()
{
    delete skill;
    
}



BattleComponent::SkillArrow::SkillArrow(const BattleConfig::SkillArrowData& arrowData, int startX, int startZ, bool destroyAfterHitHero) :
                                        arrowData(arrowData), startX(startX), startZ(startZ), destroyAfterHitHero(destroyAfterHitHero)
{
    hitHero = false;
    
    std::string eventName = AUDIOEVENTS_ARROWSKILL;
    std::string triggerName = "skill_";
    triggerName += cocos2d::StringUtils::format("%d", arrowData.skillId);
    BattleEvent::BattleSound soundEvent(eventName, triggerName, entityx::Entity());
    _ENTITY_EVENT.emit(soundEvent);
    m_audioId = *soundEvent.pSoundId;
    
    if(arrowData.type == BattleConfig::ARROW_SKILL_CURVE)
    {
        this->phase = arrowData.phase;
    }
}

BattleComponent::SkillArrow::~SkillArrow()
{
    AudioManager::getInstance()->stopEffect(m_audioId);
    
}

