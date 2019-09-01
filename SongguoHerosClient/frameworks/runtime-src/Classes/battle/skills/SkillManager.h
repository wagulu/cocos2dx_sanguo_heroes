//
//  SkillManager.h
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//

#ifndef __SKILL_MANAGER__
#define __SKILL_MANAGER__

#include <map>
#include <functional>
#include <exception>
#include <stdexcept>
#include <string>

#include <memory>
#include "SkillObject.h"
#include "SkillAction.h"
#include "SkillConfig.h"
#include "../BattleConfig.h"

class SkillManager
{
private:
    
    class ActionBuilderBase
    {
    public:
        virtual ~ActionBuilderBase() {};
        virtual SkillAction* buildAction() = 0;
    };
    
    template<class ActionClass>
    class ActionBuilder : public ActionBuilderBase
    {
    public:
        ActionBuilder() {};
        ~ActionBuilder() {};
        SkillAction* buildAction() override { return new ActionClass(); };
    };

public:
    
    SkillManager(void);
    ~SkillManager(void);
    
    static SkillManager* getInstance();
    
    SkillObject* createSkill(BattleConfig::SkillData* skillData, int fromId, const entityx::Entity::Id& entityId);
    inline BattleConfig::SkillConfigData* getSkillConfig(const std::string& name) { return m_config.getSkillConfig(name); };
    SkillAction* buildAction(const std::string& actionName);

    void initBuffEffect(const std::string& path) { m_config.initBuffEffect(path); };
    BattleConfig::EffectGroup* getBuffEffect(const std::string& name) { return m_config.getBuffEffect(name); };

private:
    
    template<typename TActionClass>
    void registerBuilder(const std::string& key);
    
    std::map<std::string, ActionBuilderBase*> m_builderMap;
    SkillConfig m_config;
    static SkillManager* s_sharedSkillManager;
};

#endif /* defined(__SKILL_MANAGER__) */
