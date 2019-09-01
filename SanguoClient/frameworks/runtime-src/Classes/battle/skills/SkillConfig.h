//
//  SkillConfig.h
//  Game
//
//  Created by fuchenhao on 2/26/15.
//
//

#ifndef __SKILL_CONFIG_H__
#define __SKILL_CONFIG_H__

#include <map>
#include "../BattleConfig.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"


class SkillConfig
{
public:
    SkillConfig();
    ~SkillConfig();
    
    void initBuffEffect(const std::string& path);
    
    BattleConfig::SkillConfigData* getSkillConfig(const std::string& configName);
    BattleConfig::EffectGroup* getBuffEffect(const std::string& name) { return m_buffEffect.find(name)->second; };
    
private:
    bool m_buffEffectLoaded = false;
    std::string m_path;
    std::map<std::string, BattleConfig::SkillConfigData*> m_skills;
    std::map<std::string, BattleConfig::EffectGroup*> m_buffEffect;
};

#endif /* defined(__SKILL_CONFIG_H__) */
