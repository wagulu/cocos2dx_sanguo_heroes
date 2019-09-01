//
//  TestBattle.h
//  sanguoClient
//
//  Created by fuchenhao on 5/15/15.
//
//

#ifndef __sanguoClient__TestBattle__
#define __sanguoClient__TestBattle__


#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

#include "assets-manager/AssetsManager.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "TestBattleConfigReader.h"
#include "BattleConfig.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "network/HttpRequest.h"
#include "json/document.h"

class TestBattleScene;

class TestBattle : public cocos2d::extension::AssetsManagerDelegateProtocol
{
public:
    friend class TestBattleScene;
    
    struct TeamData
    {
        std::string heroTestFileName;
        std::string soldierTestFileName;
        std::string formation;
        std::string ai;
        std::string pos;
        std::vector<std::string*> skills;
        
        std::string gd_heroId;
        std::string gd_heroSubType;
        std::string gd_soldierId;
        std::string gd_soldierSubType;
        
        rapidjson::Value* gd_heroJSON = nullptr;
        
        void clearSkills()
        {
            for (auto* pStr : skills)
            {
                CC_SAFE_DELETE(pStr);
            }
            skills.clear();
        }
        
        ~TeamData()
        {
            clearSkills();
        }
    };
    
    struct TestData
    {
        std::string fightName;
        TeamData myTeamData;
        TeamData enemyTeamData;
    };

    static TestBattle* getInstance();
    
    TestBattle();
    ~TestBattle();
    
    virtual void onError(cocos2d::extension::AssetsManager::ErrorCode errorCode) override;
    virtual void onProgress(int percent) override;
    virtual void onSuccess() override;
    
    cocos2d::Scene* createScene();
    cocos2d::ui::Button* createShowPanelButton();
    
    void updateBattleConfig();
    void updateTeamConfig(TeamData& teamData, BattleConfig::TeamConfig& teamConfig, TestBattleConfigReader::FileNamesResult& heroResult);
    
    void updateBattleConfigFromMonsterJson(std::string& content);
    
    void createBattleConfigData(rapidjson::Value& toData, rapidjson::Value& fromData, rapidjson::Document& doc, rapidjson::Value* pJSON);
    void addMemberToData(rapidjson::Value& toData, rapidjson::Value& fromValue, const std::string& key, rapidjson::Value& value, rapidjson::Document& doc);
    
    void startBattle();
    void endBattle();
    
    void loadVersion();
    void onLoadVersionCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    TestData testData;

private:
    void onTouchShowPanelButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    static TestBattle* s_instance;
    
    cocos2d::extension::AssetsManager* m_pAssetsManager = nullptr;
    TestBattleScene* m_pScene = nullptr;
    std::string m_configPath;
    std::string m_version;
};

class TestBattleScene : public cocos2d::Layer
{
public:
    friend class TestBattle;
    
    enum class Step
    {
        CONFIG_MY_TEAM,
        CONFIG_ENEMY_TEAM,
        START_BATTLE
    };
    
    enum class UseMode
    {
        TESTER,
        DESIGNER
    };
    
    TestBattleScene();
    CREATE_FUNC(TestBattleScene);
    
    virtual void onExit();
    
    void onVersionLoadFail();
    void onConfigLoadBegin();
    void onConfigLoadFail(const std::string& msg);
    void onConfigLoadSuccess();
    void initData();
    void updateUIDelay(float dt);
    
private:
    void initListView(cocos2d::ui::ListView* pListView, TestBattleConfigReader::FileNamesResult& result);
    void initGDListView(cocos2d::ui::ListView* pListView, std::vector<std::string>& values);
    
    void onTouchYesToLoadConfig(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void onTouchNotToLoadConfig(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void onTouchPageButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void onTouchGDFilterClearButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void onSelectListViewItem(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
    void updateUI();
    void updateTextByTeamData(cocos2d::ui::Text* pText, const TestBattle::TeamData& teamData);
    void changeStep(Step newStep);
    
    cocos2d::Node* m_pLayout = nullptr;
    cocos2d::ui::LoadingBar* m_pLoadingBar = nullptr;
    cocos2d::ui::Text* m_pLoadingText = nullptr;
    cocos2d::ui::Text* m_pLoadingTip = nullptr;
    cocos2d::ui::Text* m_pTextMyDetail = nullptr;
    cocos2d::ui::Text* m_pTextEnemyDetail = nullptr;
    cocos2d::ui::Text* m_pTextPage = nullptr;
    cocos2d::ui::Text* m_pTextFight = nullptr;
    cocos2d::ui::Button* m_pBtnPre = nullptr;
    cocos2d::ui::Button* m_pBtnNext = nullptr;
    
    cocos2d::ui::ListView* m_pListViewFight = nullptr;
    cocos2d::ui::ListView* m_pListViewHero = nullptr;
    cocos2d::ui::ListView* m_pListViewFormation = nullptr;
    cocos2d::ui::ListView* m_pListViewSkill = nullptr;
    cocos2d::ui::ListView* m_pListViewSoldier = nullptr;
    cocos2d::ui::ListView* m_pListViewAI = nullptr;
    cocos2d::ui::ListView* m_pListViewPos = nullptr;
    
    cocos2d::ui::ListView* m_pListViewGDHeroName = nullptr;
    cocos2d::ui::ListView* m_pListViewGDHeroLevel = nullptr;
    cocos2d::ui::ListView* m_pListViewGDHeroSubType = nullptr;
    cocos2d::ui::ListView* m_pListViewGDHero = nullptr;
    cocos2d::ui::ListView* m_pListViewGDFormation = nullptr;
    cocos2d::ui::ListView* m_pListViewGDPos = nullptr;
    cocos2d::ui::ListView* m_pListViewGDAI = nullptr;
    
    std::vector<cocos2d::ui::Text*> m_gdTexts;
    
    TestBattleConfigReader::FileNamesResult m_fightResult;
    TestBattleConfigReader::FileNamesResult m_heroResult;
    TestBattleConfigReader::FileNamesResult m_enemyHeroResult;
    TestBattleConfigReader::FileNamesResult m_formationResult;
    TestBattleConfigReader::FileNamesResult m_skillResult;
    TestBattleConfigReader::FileNamesResult m_soldierResult;
    TestBattleConfigReader::FileNamesResult m_aiResult;
    TestBattleConfigReader::FileNamesResult m_posResult;
    
    TestBattleConfigReader::CSVHeroResult m_csvHeroResult;
    TestBattleConfigReader::CSVSoldierResult m_csvSoldierResult;
    
    TestBattleConfigReader::MonsterResult m_monsterResult;
    std::vector<std::string> m_monsterNames;
    std::vector<std::string> m_monsterLevels;
    std::vector<std::string> m_monsterSubTypes;
    std::vector<std::string> m_monsterNamesFiltered;
    std::string m_selectedName;
    std::string m_selectedLevel;
    std::string m_selectedSubType;
    std::string m_selectedFormation;
    std::string m_selectedPos;
    std::string m_selectedAI;
    
    Step m_currentStep = Step::CONFIG_MY_TEAM;
    UseMode m_useMode = UseMode::TESTER;
};

#endif

#endif /* defined(__sanguoClient__TestBattle__) */
