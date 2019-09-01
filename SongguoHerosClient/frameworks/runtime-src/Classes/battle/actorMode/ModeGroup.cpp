//
//  ModelGroup.cpp
//  Game
//
//  Created by fu.chenhao on 3/5/15.
//
//

#include "ModeGroup.h"
#include "BattleWorld.h"

ModeGroup::ModeGroup(entityx::Entity& entity):
m_entity(entity)
{
    int total = ActorMode::TOTAL;
    for(int i = 0; i < total; ++i)
    {
        ActorMode::ModeData* pData = new ActorMode::ModeData((ActorMode::MODE_TYPE)i, 0);
        m_modeDataList[i] = pData;
    }
}


ModeGroup::~ModeGroup()
{
    int total = ActorMode::TOTAL;
    for(int i = 0; i < total; ++i)
    {
        delete m_modeDataList[i];
        m_modeDataList[i] = NULL;
    }
}

void ModeGroup::update(float dt)
{
    int total = ActorMode::TOTAL;
    for(int i = 0; i < total; ++i)
    {
        if(m_modeDataList[i]->m_actived)
        {
            m_modeDataList[i]->m_leftTime -= dt;
            if(m_modeDataList[i]->m_leftTime <= 0)
            {
                m_modeDataList[i]->m_actived = false;
                _ENTITY_EVENT.emit(BattleEvent::PlayActorModeAnim(m_entity, (ActorMode::MODE_TYPE)i, false));
            }
        }
    }
    
}


void ModeGroup::addMode(ActorMode::MODE_TYPE type, float duration)
{
    int total = ActorMode::TOTAL;

    for(int i = 0; i < total; ++i)
    {
        if(m_modeDataList[i]->m_actived
           && ActorMode::isModeA_block_ModeB(m_modeDataList[i]->m_type, type))
        {
            return;
        }
    }
    
    ActorMode::ModeData* pTargetData = m_modeDataList[type];
    if(pTargetData->m_actived)
    {
        pTargetData->m_leftTime = fmax(pTargetData->m_leftTime, duration);
    }
    else
    {
        pTargetData->m_leftTime = duration;
    }
    
    _ENTITY_EVENT.emit(BattleEvent::PlayActorModeAnim(m_entity, type, true));
//    _ENTITY_EVENT.emit(BattleEvent::SwitchActorMode(m_entity, type));
    
}


void ModeGroup::delMode(ActorMode::MODE_TYPE type)
{
    ActorMode::ModeData* pTargetData = m_modeDataList[type];
    pTargetData->m_actived = false;
    
}

