//
//  AIManager.h
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#ifndef __AI_MANAGER_H__
#define __AI_MANAGER_H__


#include <map>
#include <functional>
#include <exception>
#include <stdexcept>
#include <string>

#include <memory>

#include "AICondition.h"
#include "AIAction.h"
#include "AIConfig.h"

class AIManager
{
private:
    
    class AIConditionBuilderBase
    {
    public:
        virtual ~AIConditionBuilderBase() {};
        virtual AICondition* build(AIConfig::AIConditionData* condition, int heroId) = 0;
    };
    
    template<class AIConditionClass>
    class AIConditionBuilder : public AIConditionBuilderBase
    {
    public:
        AIConditionBuilder() {};
        ~AIConditionBuilder() {};
        AICondition* build(AIConfig::AIConditionData* condition, int heroId) override
        {
            return new AIConditionClass(condition, heroId);
        }
    };
    
    class AIActionBuilderBase
    {
    public:
        virtual ~AIActionBuilderBase() {};
        virtual AIAction* build(AIConfig::AIActionData* action, int heroId) = 0;
    };
    
    template<class AIActionClass>
    class AIActionBuilder : public AIActionBuilderBase
    {
    public:
        AIActionBuilder() {};
        ~AIActionBuilder() {};
        AIAction* build(AIConfig::AIActionData* action, int heroId) override
        {
            return new AIActionClass(action, heroId);
        }
    };
    
public:
    
    AIManager();
    ~AIManager();
    
    static AIManager* getInstance();
    void createAI(const std::string& name, int heroId);
    
    template<typename TAIConditionClass>
    void registerAICondition(const std::string& key);
    
    template<typename TAIActionClass>
    void registerAIAction(const std::string& key);
    
private:
    
    AICondition* buildCondition(AIConfig::AIConditionData* data, int heroId);
    AIAction* buildAction(AIConfig::AIActionData* data, int heroId);
    
    std::map<std::string, AIConditionBuilderBase*> m_conditionMap;
    std::map<std::string, AIActionBuilderBase*> m_actionMap;
    AIConfig m_config;
    static AIManager* s_sharedAIManager;
};


#endif /* defined(__AI_MANAGER_H__) */
