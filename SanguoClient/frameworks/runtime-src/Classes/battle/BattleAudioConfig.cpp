//
//  BattleAudioConfig.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 6/3/15.
//
//

#include "BattleAudioConfig.h"
#include "SkillManager.h"
#include "EnumParser.h"


BattleAudioConfig* BattleAudioConfig::m_instance = NULL;

BattleAudioConfig::BattleAudioConfig()
{
    
}


BattleAudioConfig::~BattleAudioConfig()
{
    m_configs.clear();
}


void BattleAudioConfig::initConfig(const std::string& configName)
{
    if(m_initializedConfigName == configName)
    {
        return;
    }
    m_initializedConfigName = configName;
 
    std::string config = cocos2d::FileUtils::getInstance()->getStringFromFile(configName);
    
    rapidjson::Document data;
    data.Parse<rapidjson::kParseDefaultFlags>(config.c_str());
    
    if(data.HasMember("sound") && data["sound"].IsArray())
    {
        int num = data["sound"].Size();
        for(int i = 0; i < num; i++)
        {
            auto& value = data["sound"][i];
            
            BattleAudioConfig::Config config;
            config.event = value["event"].GetString();
            config.trigger = value["trigger"].GetString();
            config.audio = value["audio"].GetString();
            config.maxCount = value["max"].GetInt();
            if(config.maxCount == 0)
            {
                config.maxCount = 10;
            }
            config.loop = value["loop"].GetBool();
            m_configs.push_back(config);
        }
    }
    

    
}

BattleAudioConfig::Config* BattleAudioConfig::getConfig(std::string event, std::string trigger)
{
    for (BattleAudioConfig::Config& config : m_configs)
    {
        if(config.event == event && config.trigger == trigger)
        {
            return &config;
        }
    }
    
    return NULL;
}
