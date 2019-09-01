//
//  AIAction.h
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#ifndef __AI_ACTION_H__
#define __AI_ACTION_H__

#include "AIConfig.h"

class AIAction
{
public:
    AIAction(AIConfig::AIActionData* data, int heroId);
    virtual ~AIAction();
    
    virtual void configure() {};
    virtual void tryAction() {};
    inline float getDelay() { return m_data->delay; };
    inline bool isComplete() { return m_completed; };
    inline void reset() { m_completed = false; };
    
protected:
    AIConfig::AIActionData* m_data;
    bool m_completed = false;
    int m_heroId;
    int m_side;
};


#endif /* defined(__AI_ACTION_H__) */
