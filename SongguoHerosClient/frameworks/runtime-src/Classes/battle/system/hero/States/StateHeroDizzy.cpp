//
//  StateHeroDizzy.cpp
//  Game
//
//  Created by fu.chenhao on 3/16/15.
//
//

#include "StateHeroDizzy.h"
#include "HeroSystem.h"
#include "AICommonHelper.h"
#include "DizzyActionChangeColor.h"
#include "DizzyActionPlayHitAnimation.h"
#include "DizzyActionPlayXuanyunEffect.h"
#include "DizzyActionStopAnimation.h"
#include "DizzyActionKnockBack.h"
#include "DizzyActionSkillGuiJiShiHun.h"
#include "DizzyActionSkillChiTianBaFangYan.h"
#include "DizzyConditionByTime.h"
#include "DizzyConditionBySkillEnd.h"
#include "DizzyConditionByActionComplete.h"
#include "DizzyActionSkillTiaoJi.h"
#include "DizzyActionSkillChuanSuo.h"
#include "BattleEvent.h"

std::map<std::string, DizzyConditionBuilderBase*> StateHeroDizzy::s_conditionMap;
std::map<std::string, DizzyActionBuilderBase*> StateHeroDizzy::s_actionMap;
bool StateHeroDizzy::s_inited = StateHeroDizzy::initConditionsAndActions();

#pragma mark -
#pragma mark static methods

bool StateHeroDizzy::initConditionsAndActions()
{
    if (!s_inited)
    {
        registerDizzyAction<DizzyActionChangeColor>("change_color");
        registerDizzyAction<DizzyActionPlayHitAnimation>("play_hit_animation");
        registerDizzyAction<DizzyActionPlayXuanyunEffect>("play_xuanyun_effect");
        registerDizzyAction<DizzyActionStopAnimation>("stop_animation");
        registerDizzyAction<DizzyActionKnockBack>("knock_back");
        registerDizzyAction<DizzyActionSkillGuiJiShiHun>("skill_gui_ji_shi_hun");
        registerDizzyAction<DizzyActionSkillChiTianBaFangYan>("skill_chi_tian_ba_fang_yan");
        registerDizzyAction<DizzyActionSkillTiaoJi>("skill_tiao_ji");
        registerDizzyAction<DizzyActionSkillChuanSuo>("skill_chuan_suo");
        
        registerDizzyCondition<DizzyConditionByTime>("by_time");
        registerDizzyCondition<DizzyConditionBySkillEnd>("by_skill_end");
        registerDizzyCondition<DizzyConditionByActionComplete>("by_action_complete");
    }
    return true;
}

template<typename DizzyConditionClass>
void StateHeroDizzy::registerDizzyCondition(const std::string& key)
{
    if (s_conditionMap.find(key) == s_conditionMap.end()) s_conditionMap[key] = new DizzyConditionBuilder<DizzyConditionClass>();
}

template<typename DizzyActionClass>
void StateHeroDizzy::registerDizzyAction(const std::string& key)
{
    if (s_actionMap.find(key) == s_actionMap.end()) s_actionMap[key] = new DizzyActionBuilder<DizzyActionClass>();
}

#pragma mark -
#pragma mark instance methods

StateHeroDizzy::StateHeroDizzy(const BattleConfig::HeroDizzyData& dizzyData, const BattleConfig::HeroDizzyConfig& dizzyConfig) : m_dizzyData(dizzyData), m_dizzyConfig(dizzyConfig)
{
    for (auto& actionConfig : m_dizzyConfig.actions)
    {
        addAction(&actionConfig);
    }
    for (auto& conditionConfig : m_dizzyConfig.conditions)
    {
        addCondition(&conditionConfig);
    }
}

StateHeroDizzy::~StateHeroDizzy()
{
    clearAllActions();
    clearAllConditions();
    
    CCLOG("exit dizzy");
}

void StateHeroDizzy::enter(entityx::Entity& entity)
{
    _ENTITY_EVENT.emit(BattleEvent::CancelSkill(entity));
    
    for (auto* pCondition : m_conditions)
    {
        pCondition->init(entity);
    }
    
    for (auto* pAction : m_actions)
    {
        pAction->enter(entity);
    }
    
    CCLOG("enter dizzy");
}

void StateHeroDizzy::exit(entityx::Entity& entity)
{
    for (auto* pAction : m_actions)
    {
        pAction->exit(entity);
    }
}

int StateHeroDizzy::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    for (auto* pAction : m_actions)
    {
        pAction->update(entity, dt);
    }
    
    for (auto* pCondition : m_conditions)
    {
        pCondition->update(entity, dt);
    }
    
    if (checkConditionComplete())
    {
        _RETURN_STATE_RESULT(OVER);
    }
    
    return m_result;
}

bool StateHeroDizzy::isDelegateState()
{
    return (m_result == WORKING && !allowBreakByAnotherDizzy());
}

bool StateHeroDizzy::isDelegateStateWorking()
{
    return isDelegateState();
}

bool StateHeroDizzy::checkConditionComplete()
{
    bool allRequiredComplete = true;
    bool hasConditionComplete = false;
    
    for (auto* pCondition : m_conditions)
    {
        if (pCondition->isComplete())
        {
            hasConditionComplete = true;
        }
        else if (pCondition->isRequired())
        {
            allRequiredComplete = false;
        }
    }
    
    return (allRequiredComplete && hasConditionComplete) || (m_conditions.size() == 0);
}

void StateHeroDizzy::clearAllActions()
{
    for (auto* p : m_actions) CC_SAFE_DELETE(p);
    m_actions.clear();
}

void StateHeroDizzy::clearAllConditions()
{
    for (auto* p : m_conditions) CC_SAFE_DELETE(p);
    m_conditions.clear();
}

void StateHeroDizzy::addCondition(BattleConfig::HeroDizzyConditionConfig* pConfig)
{
    auto it = s_conditionMap.find(pConfig->name);
    if (it != s_conditionMap.end())
    {
        auto* pCondition = it->second->build();
        pCondition->setData(&m_dizzyData, pConfig);
        m_conditions.push_back(pCondition);
    }
}

void StateHeroDizzy::addAction(BattleConfig::HeroDizzyActionConfig* pConfig)
{
    auto it = s_actionMap.find(pConfig->name);
    if (it != s_actionMap.end())
    {
        auto* pAction = it->second->build();
        pAction->setData(&m_dizzyData, pConfig);
        m_actions.push_back(pAction);
    }
}
