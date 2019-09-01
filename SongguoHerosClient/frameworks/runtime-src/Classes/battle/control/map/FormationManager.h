//
//  FormationManager.h
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#ifndef __FORMATION_MANAGER_H__
#define __FORMATION_MANAGER_H__

#include "cocos2d.h"
#include "BattleConfig.h"

class FormationManager
{
public:
    
    FormationManager(void);
    ~FormationManager(void);
    
    BattleConfig::FormationConfig& getFormation(const std::string name);
    static FormationManager* getInstance();
    
private:
    
    void split(const std::string &str, const std::string& seperator, std::vector<std::string>& ret);
    
    std::string m_path;
    std::map<std::string, BattleConfig::FormationConfig*> m_formation;
    static FormationManager* s_sharedFormationManager;
    
};

#endif /* defined(__FORMATION_MANAGER_H__) */
