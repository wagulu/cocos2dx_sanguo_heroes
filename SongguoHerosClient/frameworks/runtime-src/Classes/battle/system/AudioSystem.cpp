//
//  AudioSystem.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 6/4/15.
//
//

#include "AudioSystem.h"
#include "AudioManager.h"
#include "BattleWorld.h"
#include "BattleComponent.h"

AudioSystem::~AudioSystem()
{
    
}

void AudioSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::BattleSound>(*this);
}

void AudioSystem::receive(const BattleEvent::BattleSound& e)
{
    if(BattleWorld::getInstance()->isAutoQuickBattleMode())
    {
        *e.pSoundId = -1;
        return;
    }

    BattleAudioConfig* pConfig = BattleAudioConfig::getInstance();
    BattleAudioConfig::Config* pSetting = pConfig->getConfig(e.name, e.triggerName);
    if(pSetting && pSetting->audio != "")
    {
        bool insideScreen = true;
        
        if(e.fireEntity.valid())
        {
            entityx::Entity fireEntity = e.fireEntity;
            auto pos = fireEntity.component<BattleComponent::Position>();
            if(pos.valid())
            {
                if(!_CAMERA_MANAGER.inCameraView(pos->x))
                {
                    insideScreen = false;
                }
                
//                float cx = _CAMERA_MANAGER.getCameraX();
//                float cz = _CAMERA_MANAGER.getCameraZ();
//                
//                float dx = fabs(pos->x - cx);
//                if(dx >= GRID_WIDTH * 20)
//                {
//                }
                
            }
            

        }
        
        if(insideScreen)
        {
            *e.pSoundId = AudioManager::getInstance()->playEffectWithLimitedConcurrency(pSetting->audio, pSetting->maxCount, pSetting->loop);
            
        }
        else
        {
//            CCLOG("miss audio file config, name = %s, trigger = %s", e.name.c_str(), e.triggerName.c_str());

        }
    }
    else
    {
//        CCLOG("miss audio config, name = %s, trigger = %s", e.name.c_str(), e.triggerName.c_str());
    }
}

void AudioSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    
}
