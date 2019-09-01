//
//  StateHeroDizzy.h
//  Game
//
//  Created by fu.chenhao on 3/16/15.
//
//

#ifndef __STATE_HERO_DIZZY_H__
#define __STATE_HERO_DIZZY_H__

#include "IState.h"
#include "BattleConfig.h"
#include "BattleComponent.h"
#include "DizzyAction.h"
#include "DizzyCondition.h"

class DizzyActionBuilderBase;
class DizzyConditionBuilderBase;

class StateHeroDizzy : public IState
{
public:
    
    enum RESULT
    {
        WORKING,
        OVER,
    };
    RESULT m_result = WORKING;
    
    StateHeroDizzy(const BattleConfig::HeroDizzyData& dizzyData, const BattleConfig::HeroDizzyConfig& dizzyConfig);
    virtual ~StateHeroDizzy();
    
    virtual int getStateCode()
    {
        return BattleConfig::GENERAL_DIZZY;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    virtual bool isDelegateState();
    virtual bool isDelegateStateWorking();
    
    inline bool allowBreakByAnotherDizzy()
    {
        return m_dizzyConfig.allowBreakByAnotherDizzy;
    }
    
    inline const std::vector<DizzyAction*>& getActions() { return m_actions; };
    
private:    
    std::vector<DizzyAction*> m_actions;
    std::vector<DizzyCondition*> m_conditions;
    BattleConfig::HeroDizzyData m_dizzyData;
    BattleConfig::HeroDizzyConfig m_dizzyConfig;
    
    static std::map<std::string, DizzyConditionBuilderBase*> s_conditionMap;
    static std::map<std::string, DizzyActionBuilderBase*> s_actionMap;
    static bool s_inited;
    
    void clearAllActions();
    void clearAllConditions();
    void addCondition(BattleConfig::HeroDizzyConditionConfig* pConfig);
    void addAction(BattleConfig::HeroDizzyActionConfig* pConfig);
    bool checkConditionComplete();
    
    static bool initConditionsAndActions();
    
    template<typename DizzyConditionClass>
    static void registerDizzyCondition(const std::string& key);
    
    template<typename DizzyActionClass>
    static void registerDizzyAction(const std::string& key);
};

class DizzyActionBuilderBase
{
public:
    virtual ~DizzyActionBuilderBase() {};
    virtual DizzyAction* build() = 0;
};

template<class DizzyActionClass>
class DizzyActionBuilder : public DizzyActionBuilderBase
{
public:
    DizzyActionBuilder() {};
    ~DizzyActionBuilder() {};
    DizzyAction* build() override { return new DizzyActionClass(); }
};

class DizzyConditionBuilderBase
{
public:
    virtual ~DizzyConditionBuilderBase() {};
    virtual DizzyCondition* build() = 0;
};

template<class DizzyConditionClass>
class DizzyConditionBuilder : public DizzyConditionBuilderBase
{
public:
    DizzyConditionBuilder() {};
    ~DizzyConditionBuilder() {};
    DizzyCondition* build() override { return new DizzyConditionClass(); }
};

#endif /* defined(__STATE_HERO_DIZZY_H__) */
