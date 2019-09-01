//
//  AIConfig.cpp
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#include "AIConfig.h"

AIConfig::AIConfig()
{
    m_path = "config/ai/";
}

AIConfig::~AIConfig()
{
    for(auto& ai : m_ai)
    {
        delete ai.second;
    }
    m_ai.clear();
}

/*******************
    "ai":
    [
        {
            "repeated":false,
            "condition":[{"name":"skill_ok", "value":0}, "condition":{"name":"battle_start"}],
            "action":{"name":"fire_skill", "delay":5, "value":0}
        }
    ]
********************/

AIConfig::AIGroup* AIConfig::getAI(const std::string name)
{
    auto result = m_ai.find(name);
    if(result != m_ai.end()) return result->second;
    
    // read battle config
    std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile(m_path + name + ".json");
    if (json.length() == 0) return nullptr;
    
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
    
    auto group = new AIGroup();
    m_ai.insert(std::pair<std::string, AIGroup*>(name, group));
    
    if(doc.HasMember("ai") && doc["ai"].IsArray())
    {
        int size = doc["ai"].Size();
        for(int i = 0; i < size; i++)
        {
            auto action = new AIConfig::AIActionData();
            auto ai = new AIConfig::AIData();
            ai->action = action;
            
            auto& value = doc["ai"][i];
            ai->repeated = value["repeated"].GetBool();
            
            if(value.HasMember("condition") && value["condition"].IsArray())
            {
                int count = value["condition"].Size();
                for(int j = 0; j < count; j++)
                {
                    auto& conditionValue = value["condition"][j];
                    auto condition = new AIConfig::AIConditionData();
                    ai->conditions.push_back(condition);
                    
                    condition->name = std::string(conditionValue["name"].GetString());
                    if(conditionValue.HasMember("value"))
                    {
                        condition->value = conditionValue["value"].GetInt();
                    }
                }
            }
            
            if(value.HasMember("action"))
            {
                auto& actionValue = value["action"];
                
                action->name = std::string(actionValue["name"].GetString());
                action->delay = actionValue["delay"].GetDouble();
                if(actionValue.HasMember("value"))
                {
                    action->value = actionValue["value"].GetInt();
                }
            }
            
            group->group.push_back(ai);
        }
    }

    return group;
}
