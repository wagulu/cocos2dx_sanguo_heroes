//
//  AIManager.cpp
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#include "AIManager.h"

#include "actions/EntityCommand.h"
#include "actions/FireSkillAction.h"

#include "conditions/BattleStart.h"
#include "conditions/SkillOk.h"
#include "conditions/SkillRelease.h"
#include "conditions/AfterAnySkillRelease.h"
#include "conditions/NoOppSkillFired.h"
#include "conditions/NoSelfSkillFired.h"
#include "conditions/HeroRush.h"
#include "conditions/HeroHpLessThan.h"
#include "conditions/HeroHpPercentLessThan.h"
#include "conditions/SoldiersLessThan.h"
#include "conditions/HeroAndHeroDistanceLessThan.h"
#include "conditions/HeroAndSoldierDistanceLessThan.h"
#include "conditions/SoldierAndHeroDistanceLessThan.h"
#include "conditions/SoldierAndSoldierDistanceLessThan.h"
#include "conditions/SelfHeroAroundOppSoldiersMoreThan.h"
#include "conditions/OppHeroAroundOppSoldiersMoreThan.h"
#include "conditions/OppHeroAroundSelfSoldiersMoreThan.h"
#include "conditions/OppHeroAroundSelfSoldiersLessThan.h"
#include "conditions/FirstSoldierKilledInMelee.h"
#include "conditions/EnemyIsClose.h"
#include "conditions/SelfMainHeroBeAttacked.h"
#include "conditions/OppSoldiersMoreThan.h"
#include "conditions/SoldiersMoreThan.h"
#include "conditions/NoHeroMove.h"
#include "conditions/HeroHpPercentMoreThan.h"

#include "BattleWorld.h"

AIManager* AIManager::s_sharedAIManager = nullptr;

AIManager::AIManager()
{
    registerAIAction<SoldierGoBack>("soldier_back");
    registerAIAction<SoldierGoCenter>("soldier_center");
    registerAIAction<SoldierGoDepart>("soldier_depart");
    registerAIAction<SoldierKillHero>("soldier_attack_hero");
    registerAIAction<SoldierKillSoldier>("soldier_attack_soldier");
    registerAIAction<SoldierIdle>("soldier_idle");
    registerAIAction<SoldierProtectHero>("soldier_guard");
    registerAIAction<HeroGo>("hero_rush");
    registerAIAction<HeroStop>("hero_stop");
    registerAIAction<FireSkillAction>("fire_skill");
    
    registerAICondition<BattleStart>("battle_start");
    registerAICondition<FirstSoldierKilledInMelee>("first_soldier_killed_in_melee");
    registerAICondition<SelfMainHeroBeAttacked>("self_main_hero_be_attacked");
    registerAICondition<HeroRush>("hero_rush");
    registerAICondition<NoHeroMove>("no_hero_move");
   
    registerAICondition<SkillOk>("skill_ok");
    registerAICondition<SkillRelease>("skill_release");
    registerAICondition<AfterAnySkillRelease>("after_any_skill_release");
    registerAICondition<NoOppSkillFired>("no_opp_skill_fired");
    registerAICondition<NoSelfSkillFired>("no_self_skill_fired");
    
    registerAICondition<HeroHpLessThan>("hero_hp_less_than");
    registerAICondition<HeroHpPercentLessThan>("hero_hp_percent_less_than");
    registerAICondition<HeroHpPercentMoreThan>("hero_hp_percent_more_than");
    
    registerAICondition<SoldiersLessThan>("soldiers_less_than");
    registerAICondition<SoldiersMoreThan>("soldiers_more_than");
    registerAICondition<OppSoldiersMoreThan>("opp_soldiers_more_than");

    registerAICondition<SelfHeroAroundOppSoldiersMoreThan>("self_hero_around_opp_soldiers_more_than");
    registerAICondition<OppHeroAroundOppSoldiersMoreThan>("opp_hero_around_opp_soldiers_more_than");
    registerAICondition<OppHeroAroundSelfSoldiersMoreThan>("opp_hero_around_self_soldiers_more_than");
    registerAICondition<OppHeroAroundSelfSoldiersLessThan>("opp_hero_around_self_soldiers_less_than");
    
    registerAICondition<HeroAndHeroDistanceLessThan>("hero_and_hero_distance_less_than");
    registerAICondition<HeroAndSoldierDistanceLessThan>("hero_and_soldier_distance_less_than");
    registerAICondition<SoldierAndHeroDistanceLessThan>("soldier_and_hero_distance_less_than");
    registerAICondition<SoldierAndSoldierDistanceLessThan>("soldier_and_soldier_distance_less_than");
    registerAICondition<EnemyIsClose>("enemy_distance_is_less_than");
}

AIManager* AIManager::getInstance()
{
    if(!s_sharedAIManager)
    {
        s_sharedAIManager = new AIManager();
    }
    return s_sharedAIManager;
}


template<typename TAIConditionClass>
void AIManager::registerAICondition(const std::string& key)
{
    auto it = m_conditionMap.find(key);
    if (it == m_conditionMap.end())
    {
        m_conditionMap[key] = new AIConditionBuilder<TAIConditionClass>();
    }
}

template<typename TAIActionClass>
void AIManager::registerAIAction(const std::string& key)
{
    auto it = m_actionMap.find(key);
    if (it == m_actionMap.end())
    {
        m_actionMap[key] = new AIActionBuilder<TAIActionClass>();
    }
}

AICondition* AIManager::buildCondition(AIConfig::AIConditionData* data, int heroId)
{
    auto it = m_conditionMap.find(data->name);
    if (it == m_conditionMap.end())
        return nullptr;
    return it->second->build(data, heroId);
}

AIAction* AIManager::buildAction(AIConfig::AIActionData* data, int heroId)
{
    auto it = m_actionMap.find(data->name);
    if (it == m_actionMap.end())
        return nullptr;
    return it->second->build(data, heroId);
}

void AIManager::createAI(const std::string& name, int heroId)
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(heroId);
    if (pEntity != nullptr)
    {
        auto* pConfig = m_config.getAI(name);
        if (pConfig != nullptr)
        {
            auto aiComp = pEntity->component<BattleComponent::AI>();
            if (aiComp->strategy == name)
            {
                CCLOG("create ai duplicated: %s", name.c_str());
                return;
            }
            
            aiComp->clearAllAI();
            aiComp->strategy = name;
            
            for(auto ai : pConfig->group)
            {
                std::vector<AICondition*> *conditions = new std::vector<AICondition*>();
                for(auto condition : ai->conditions)
                {
                    conditions->push_back(buildCondition(condition, heroId));
                }
                auto* pData = new BattleConfig::AIData(conditions, buildAction(ai->action, heroId), ai->repeated);
                aiComp->ai.push_back(pData);
            }
        }
    }
}

