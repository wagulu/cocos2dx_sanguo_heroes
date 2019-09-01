//
//  BattleResult.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/28/15.
//
//

#include "BattleResult.h"
#include "BattleWorld.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

void BattleResult::prepare()
{
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    
    rapidjson::Value dataMyHeros(rapidjson::kArrayType);
    prepairHeroResult(BattleConfig::SIDE_LEFT, dataMyHeros, allocator);
    
    rapidjson::Value dataEnemyHeros(rapidjson::kArrayType);
    prepairHeroResult(BattleConfig::SIDE_RIGHT, dataEnemyHeros, allocator);
    
    rapidjson::Value dataMySoldiers(rapidjson::kArrayType);
    prepairSoldierResult(_BATTLE_ENTITY.getLeftSoldiers(), dataMySoldiers, allocator);
    
    rapidjson::Value dataEnemySoldiers(rapidjson::kArrayType);
    prepairSoldierResult(_BATTLE_ENTITY.getRightSoldiers(), dataEnemySoldiers, allocator);
    
    rapidjson::Value myTeam(rapidjson::kObjectType);
    myTeam.AddMember("heros", dataMyHeros, allocator);
    myTeam.AddMember("soldiers", dataMySoldiers, allocator);
    
    rapidjson::Value enemyTeam(rapidjson::kObjectType);
    enemyTeam.AddMember("heros", dataEnemyHeros, allocator);
    enemyTeam.AddMember("soldiers", dataEnemySoldiers, allocator);
    
    m_win = !m_leftHeroAllDead && m_rightHeroAllDead;
    
    rapidjson::Value root(rapidjson::kObjectType);
    root.AddMember("win", m_win, allocator);
    root.AddMember("myTeam", myTeam, allocator);
    root.AddMember("enemyTeam", enemyTeam, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    root.Accept(writer);
    
    m_result = string(buffer.GetString());
}

void BattleResult::setTimeUp()
{
    m_reason = Reason::TIME_UP;
    
    setMyTeamAllDead();
    setEnemyTeamAllDead();
}

void BattleResult::setExitBeforeEnd()
{
    m_reason = Reason::EXIT_BEFORE_END;
    
    setMyTeamAllDead();
    recordHeroProperty(RIGHT_HERO);
}

void BattleResult::setHeroDead(int heroId)
{
    m_reason = Reason::HERO_DEAD;
    
    recordHeroProperty(LEFT_HERO);
    recordHeroProperty(RIGHT_HERO);
}

void BattleResult::setMyTeamAllDead()
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    
    for (auto* pHeroConfig : battleFieldConfig.myTeam.heros)
    {
        pHeroConfig->heroConfigData.currentHP = 0;
        pHeroConfig->heroConfigData.currentMP = 0;
    }
}

void BattleResult::setEnemyTeamAllDead()
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    
    for (auto* pHeroConfig : battleFieldConfig.enemyTeam.heros)
    {
        pHeroConfig->heroConfigData.currentHP = 0;
        pHeroConfig->heroConfigData.currentMP = 0;
    }
}

void BattleResult::recordHeroProperty(int heroId)
{
    auto* pEntity = _BATTLE_ENTITY.getEntityEvenIfDead(heroId);
    if (pEntity)
    {
        auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
        auto& heros = (MapHelper::getSide(heroId) == BattleConfig::Side::SIDE_LEFT ? battleFieldConfig.myTeam.heros : battleFieldConfig.enemyTeam.heros);
        
        auto hero = pEntity->component<BattleComponent::General>();
        auto property = pEntity->component<BattleComponent::Property>();
        
        for (auto* pHeroConfig : heros)
        {
            if (pHeroConfig->heroConfigData.id == hero->config.id)
            {
                pHeroConfig->heroConfigData.currentHP = property->get(BattleConfig::Property::HP);
                pHeroConfig->heroConfigData.currentMP = property->get(BattleConfig::Property::MP);
                break;
            }
        }
    }
}

void BattleResult::prepairHeroResult(BattleConfig::Side side, rapidjson::Value& data, rapidjson::Document::AllocatorType& allocator)
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    auto& heroConfigs = (side == BattleConfig::SIDE_LEFT ? battleFieldConfig.myTeam.heros : battleFieldConfig.enemyTeam.heros);
    
    bool allHeroDead = true;
    
    for (auto* pHeroConfig : heroConfigs)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("id", pHeroConfig->heroConfigData.id, allocator);
        obj.AddMember("hp", pHeroConfig->heroConfigData.currentHP, allocator);
        obj.AddMember("mp", pHeroConfig->heroConfigData.currentMP, allocator);
        
        data.PushBack(obj, allocator);
        
        if (pHeroConfig->heroConfigData.currentHP > 0)
        {
            allHeroDead = false;
        }
    }
    
    if (side == BattleConfig::SIDE_LEFT)
    {
        m_leftHeroAllDead = allHeroDead;
    }
    else
    {
        m_rightHeroAllDead = allHeroDead;
    }
}

void BattleResult::prepairSoldierResult(std::vector<entityx::Entity>& entities, rapidjson::Value& data, rapidjson::Document::AllocatorType& allocator)
{
    map<int, int> mapSoldier;
    
    for (auto& entity : entities)
    {
        auto soldier = entity.component<BattleComponent::Soldier>();
        
        auto iter = mapSoldier.find(soldier->config.id);
        if (iter != mapSoldier.end())
        {
            iter->second++;
        }
        else
        {
            mapSoldier.insert(pair<int, int>(soldier->config.id, 1));
        }
    }
    
    for(auto& iter : mapSoldier)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("id", iter.first, allocator);
        obj.AddMember("num", iter.second, allocator);
        
        data.PushBack(obj, allocator);
    }
}
