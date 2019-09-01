//
//  AIConfig.h
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#ifndef __AI_CONFIG_H__
#define __AI_CONFIG_H__

#include <map>
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"

class AIConfig
{
public:
    struct AIConditionData
    {
        std::string name;
        int value;
        
        AIConditionData()
        {
            value = 0;
        }
    };
    
    struct AIActionData
    {
        std::string name;
        int value;
        float delay;
        
        AIActionData()
        {
            value = 0;
            delay = 0;
        }
    };
    
    struct AIData
    {
        std::vector<AIConditionData*> conditions;
        AIActionData* action;
        bool repeated;
        
        ~AIData()
        {
            for(auto condition : conditions)
            {
                delete condition;
            }
            conditions.clear();
            
            delete action;
        }
    };
    
    struct AIGroup
    {
        std::vector<AIData*> group;
        
        ~AIGroup()
        {
            for(auto ai : group)
            {
                delete ai;
            }
            group.clear();
        }
    };
    
    AIConfig();
    ~AIConfig();
    
    AIGroup* getAI(const std::string name);
    
private:
    std::string m_path;
    std::map<std::string, AIGroup*> m_ai;
};

#endif /* defined(__AI_CONFIG_H__) */
