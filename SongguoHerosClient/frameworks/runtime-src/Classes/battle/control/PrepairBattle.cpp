//
//  PrepairBattle.cpp
//  Game
//
//  Created by fuchenhao on 3/6/15.
//
//

#include "PrepairBattle.h"
#include "BattleWorld.h"
#include "CCMyShader.h"
#include "../../asset/CCAssetDefineCache.h"

using namespace std;

void PrepairBattle::prepair()
{
    CCMyShader::initSharder();
    
    auto* prepare = new PrepairBattle();
    prepare->start();
}

void PrepairBattle::start()
{
    auto* scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->scheduleUpdate(this, 0, false);
    
    m_currentStep = 0;
    m_currentAsset = 0;
    
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    
    collectSoldierAsset(battleFieldConfig.myTeam.soldier, battleFieldConfig.leftSoldierAssetStyle);
    collectSoldierAsset(battleFieldConfig.enemyTeam.soldier, battleFieldConfig.rightSoldierAssetStyle);
    
    collectHeroAsset(battleFieldConfig.myTeam.heros, battleFieldConfig.leftSoldierAssetStyle);
    collectHeroAsset(battleFieldConfig.enemyTeam.heros, battleFieldConfig.rightSoldierAssetStyle);
}

void PrepairBattle::collectSoldierAsset(BattleConfig::SoldierConfig& soldierConfig, const std::string& soldierStyle)
{
    m_assetSoldiers.push_back(soldierConfig.getAssetName(soldierStyle));
}

void PrepairBattle::collectHeroAsset(std::vector<BattleConfig::HeroConfig*>& heros, const std::string& soldierStyle)
{
    for (auto* pHeroConfig : heros)
    {
        m_assetHeros.push_back(pHeroConfig->getAssetName());
        
        auto& skills = pHeroConfig->skills;
        for (auto* pSkillData : skills)
        {
            if (pSkillData->pSoldier != nullptr)
            {
                collectSoldierAsset(*pSkillData->pSoldier, soldierStyle);
            }
            if (pSkillData->pConfig->effect->asset != "")
            {
                m_assetSkills.push_back(pSkillData->pConfig->effect->asset);
            }
        }
    }
}

void PrepairBattle::update(float dt)
{
    switch(m_currentStep)
    {
        case 0://25~35
        if(m_currentAsset < m_assetSoldiers.size())
        {
            auto& asset = m_assetSoldiers[m_currentAsset];
            cocos2d::AssetDefineCache::getInstance()->addAssetWithFile(m_pathSolder + asset + ".png", m_pathSolder + asset + ".dat", asset);
            _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(15.0 + ((float)(m_currentAsset + 1)) / ((float)m_assetSoldiers.size())  * 10.0f);
            m_currentAsset++;
        }
        else
        {
            m_currentStep++;
            m_currentAsset = 0;
        }
        break;
        
        case 1://35~45
        if(m_currentAsset < m_assetHeros.size())
        {
            auto& asset = m_assetHeros[m_currentAsset];
            cocos2d::AssetDefineCache::getInstance()->addAssetWithFile(m_pathHero + asset + ".png", m_pathHero + asset + ".dat", asset);
            _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(25.0 + ((float)(m_currentAsset + 1)) / ((float)m_assetHeros.size())  * 10.0f);
            m_currentAsset++;
        }
        else
        {
            m_currentStep++;
            m_currentAsset = 0;
        }
        break;
        
        case 2://45~65
        if(m_currentAsset < m_assetSkills.size())
        {
            auto& asset = m_assetSkills[m_currentAsset];
            cocos2d::AssetDefineCache::getInstance()->addAssetWithFile(m_pathSkill + asset + ".png", m_pathSkill + asset + ".dat", asset);
            _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(35.0 + ((float)(m_currentAsset + 1)) / ((float)m_assetSkills.size())  * 20.0f);
            m_currentAsset++;
        }
        else
        {
            m_currentStep++;
            m_currentAsset = 0;
        }
        break;

        case 3:
        cocos2d::AssetDefineCache::getInstance()->addAssetWithFile("png/effect/battleEffect.png", "png/effect/battleEffect.dat", "battleEffect");
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(57);
        m_currentStep++;
        break;
        
        case 4:
        cocos2d::AssetDefineCache::getInstance()->addAssetWithFile("png/effect/attack.png", "png/effect/attack.dat", "attack");
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(59);
        m_currentStep++;
        break;
        
        case 5:
        cocos2d::AssetDefineCache::getInstance()->addAssetWithFile("png/effect/damage.png", "png/effect/damage.dat", "damage");
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(61);
        m_currentStep++;
        break;
        
        case 6:
        cocos2d::AssetDefineCache::getInstance()->addAssetWithFile("png/bg/decoration1.png", "png/bg/decoration1.dat", "scene_decoration1");
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(63);
        m_currentStep++;
        break;
        
        case 7:
        cocos2d::AssetDefineCache::getInstance()->addAssetWithFile("png/bg/decoration2.png", "png/bg/decoration2.dat", "scene_decoration2");
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(65);
        m_currentStep++;
        break;
        
        case 8:
        cocos2d::AssetDefineCache::getInstance()->addAssetWithFile("png/bg/closeup.png", "png/bg/closeup.dat", "scene_closeup");
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(67);
        m_currentStep++;
        break;
        
        case 9:
        prepairEntity();
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(85);
        m_currentStep++;
        break;
        
        case 10:
        prepairBackground();
        _ENTITY_EVENT.emit<BattleEvent::StartBattleProgress>(99);
        m_currentStep++;
        break;
        
        case 11:
        _ENTITY_EVENT.emit<BattleEvent::StartBattleComplete>();
        m_currentStep++;
        break;
        
        default:
        auto* scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->unscheduleUpdate(this);
        delete this;
        break;
    }
}

void PrepairBattle::prepairBackground()
{
    auto bg = BattleConfig::getInstance()->getBattleFieldConfig().background;
    _BACKGROUND_CREATOR.addBackground(bg + "/floor.png", bg + "/bg1.png", bg + "/bg2_1.png", bg + "/bg2_2.png", bg + "/front.png");
}

void PrepairBattle::prepairEntity()
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    
    addSoldier(BattleConfig::SIDE_LEFT);
    addSoldier(BattleConfig::SIDE_RIGHT);
    
    int i = 0;
    for (auto* pHeroConfig : battleFieldConfig.myTeam.heros)
    {
        bool asBackup = (i != 0);
        addHero(LEFT_HERO, *pHeroConfig, asBackup);
        i++;
    }
    i = 0;
    for (auto* pHeroConfig : battleFieldConfig.enemyTeam.heros)
    {
        bool asBackup = (i != 0);
        addHero(RIGHT_HERO, *pHeroConfig, asBackup);
        i++;
    }
}

void PrepairBattle::addSoldier(BattleConfig::Side side)
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    auto& teamConfig = battleFieldConfig.getTeam(side);
    auto& soldierConfig = teamConfig.soldier;
    auto& formationConfig = FormationManager::getInstance()->getFormation(teamConfig.formation);
    
    int tileZ, tileX;

    for (int i = 0; i < soldierConfig.num; i++)
    {
        auto& pos = formationConfig.getSoldierPos(i);
        if (pos.x >= 0 && pos.z >= 0)
        {
            tileZ = pos.z;
            if (side == BattleConfig::Side::SIDE_LEFT) tileX = pos.x + battleFieldConfig.xOffset;
            else tileX = STAGE_WIDTH - pos.x - battleFieldConfig.xOffset;
            
            if (battleFieldConfig.battleType == BattleConfig::BattleType::DUEL)
            {
                _SOLDIER_CREATOR.addBackgroundSoldier(side, soldierConfig, tileZ, tileX);
            }
            else
            {
                _SOLDIER_CREATOR.addSoldier(side, soldierConfig, tileZ, tileX);
            }
        }
    }
}

void PrepairBattle::addHero(int heroId, BattleConfig::HeroConfig& heroConfig, bool asBackup)
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    auto side = MapHelper::getSide(heroId);
    auto& teamConfig = battleFieldConfig.getTeam(side);
    auto& formationConfig = FormationManager::getInstance()->getFormation(teamConfig.formation);
    
    auto& pos = formationConfig.getHeroPos(heroConfig.heroConfigData.pos);
    if (pos.x >= 0)
    {
        int tileZ = pos.z;
        int tileX = (side == BattleConfig::SIDE_LEFT ? pos.x + battleFieldConfig.xOffset : STAGE_WIDTH - pos.x - battleFieldConfig.xOffset);
        
        if (asBackup)
        {
            _HERO_CREATOR.addBackupHero(side, heroConfig, tileZ, tileX);
        }
        else
        {
            _HERO_CREATOR.addHero(heroId, heroConfig, tileZ, tileX);
        }
    }
}
