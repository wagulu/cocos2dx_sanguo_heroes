//
//  AICondition.h
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#ifndef __AI_CONDITION_H__
#define __AI_CONDITION_H__

#include "entityx.h"
#include "AIConfig.h"

#define MAX_COUNT       30

class AICondition
{
public:
    AICondition(AIConfig::AIConditionData* data, int heroId);
    virtual ~AICondition();
    
    virtual void configure() {};
    virtual bool isOk() { return m_ok; };
    inline void reset() { m_ok = false; };

protected:
    
    bool shouldCheck();
    
    AIConfig::AIConditionData* m_data;
    int m_heroId;
    int m_side;
    bool m_ok = false;
    int m_count;
};


#endif /* defined(__Game__AICondition__) */
