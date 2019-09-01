//
//  BattleAudioConfig.h
//  sanguoClient
//
//  Created by fu.chenhao on 6/3/15.
//
//

#ifndef __sanguoClient__BattleAudioConfig__
#define __sanguoClient__BattleAudioConfig__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class BattleAudioConfig
{
public:
    struct Config
    {
        std::string event;
        std::string trigger;
        std::string audio;
        int maxCount;
        bool loop;
    };
    
    BattleAudioConfig();
    ~BattleAudioConfig();
    
    void initConfig(const std::string& config);

    BattleAudioConfig::Config* getConfig(std::string event, std::string trigger);
    
    static BattleAudioConfig* getInstance()
    {
        if(m_instance == NULL) m_instance = new BattleAudioConfig();
        return m_instance;
    }
    
private:
    std::string m_initializedConfigName;
    std::vector<Config> m_configs;
    
    static BattleAudioConfig* m_instance;
    
};

#endif /* defined(__sanguoClient__BattleAudioConfig__) */
