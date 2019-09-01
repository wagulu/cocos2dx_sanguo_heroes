//
//  ModelGroup.h
//  Game
//
//  Created by fu.chenhao on 3/5/15.
//
//

#ifndef __Game__ModeGroup__
#define __Game__ModeGroup__

#include <stdio.h>
#include "entityx.h"
#include "ActorMode.h"

class ModeGroup
{
public:
    ModeGroup(entityx::Entity& entity);
    ~ModeGroup();
    
    void update(float dt);
    
    void addMode(ActorMode::MODE_TYPE type, float duration);
    void delMode(ActorMode::MODE_TYPE type);
    
    const ActorMode::ModeData& getModeData(ActorMode::MODE_TYPE type) { return *(m_modeDataList[type]); }
    bool isModeActive(ActorMode::MODE_TYPE type) { return m_modeDataList[type]->m_actived; }
    
private:
    entityx::Entity& m_entity;
    ActorMode::ModeData* m_modeDataList[ActorMode::TOTAL];
    
    
};


#endif /* defined(__Game__ModelGroup__) */
