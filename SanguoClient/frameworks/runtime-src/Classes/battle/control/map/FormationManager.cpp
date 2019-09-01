//
//  FormationManager.cpp
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#include "FormationManager.h"
#include "../../BattleConfig.h"

FormationManager* FormationManager::s_sharedFormationManager = nullptr;

FormationManager* FormationManager::getInstance()
{
    if(!s_sharedFormationManager)
    {
        s_sharedFormationManager = new FormationManager();
    }
    return s_sharedFormationManager;
}

FormationManager::FormationManager()
{
    m_path = "config/formation/";
}

BattleConfig::FormationConfig& FormationManager::getFormation(const std::string name)
{
    auto iter = m_formation.find(name);
    if(iter != m_formation.end())
    {
        return *iter->second;
    }
    
    // read formation config
    std::string str = cocos2d::CCFileUtils::getInstance()->getStringFromFile(m_path + name + ".csv");
    std::vector<std::string> ret;
    split(str, "\r\n", ret);
    
    BattleConfig::FormationConfig* config = new BattleConfig::FormationConfig();
    m_formation.insert(std::pair<std::string, BattleConfig::FormationConfig*>(name, config));
    
    config->validSoldiersNum = 0;
    
    std::vector<std::vector<std::string>> gridData;
    for (size_t i = 0; i < ret.size(); ++i)
    {
        std::vector<std::string> fields;
        split(ret[i], ",", fields);
        gridData.push_back(fields);
    }
    
    int current = 0;
    for(int i = 0; i < gridData.size(); i++)
    {
        for(int j = 1; j < gridData[i].size(); j++)
        {
            auto& data = gridData[i][j];
            if(data != "")
            {
                int pos = (((STAGE_HEIGHT - 1 - i) << 8) | j);
                
                if (data.find("f") != std::string::npos || data.find("b") != std::string::npos)
                {
                    config->heros.insert(std::pair<std::string, int>(data, pos));
                }
                else
                {
                    config->validSoldiersNum++;
                    current = atoi(data.c_str());
                    config->soldiers.at(current - 1) = pos;
                }
            }
        }
    }
    gridData.clear();
    
    return *config;
}

void FormationManager::split(const std::string &str, const std::string& seperator, std::vector<std::string>& ret)
{
    ret.clear();
    
    size_t nStartPosFound = str.find(seperator, 0);
    size_t nFieldStart = 0;
    for (; nStartPosFound != -1; nStartPosFound = str.find(seperator, nStartPosFound))
    {
        std::string strSub = str.substr(nFieldStart, nStartPosFound - nFieldStart);
        nStartPosFound = nStartPosFound + seperator.size();
        nFieldStart = nStartPosFound;
        
        ret.push_back(strSub);
    }
    
    if (nFieldStart < str.size())
    {
        std::string strSub = str.substr(nFieldStart, str.size() - nFieldStart);
        ret.push_back(strSub);
    }
}

