//
//  TestBattleConfigLoader.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/15/15.
//
//

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

#include "TestBattle.h"
#include "BattleScene.h"
#include "BattleWorld.h"
#include "SkillManager.h"
#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpClient.h"
#include "external/json/rapidjson.h"
#include "external/json/stringbuffer.h"
#include "external/json/writer.h"
#include "external/json/prettywriter.h"

TestBattle* TestBattle::s_instance = nullptr;

TestBattle* TestBattle::getInstance()
{
    if(!s_instance)
    {
        s_instance = new TestBattle();
    }
    return s_instance;
}

TestBattle::TestBattle()
{
   m_configPath = "http://static.joytouching.com/builds/ios/sanguo_config/";
}

TestBattle::~TestBattle()
{
    CC_SAFE_DELETE(m_pAssetsManager);
    CC_SAFE_DELETE(m_pScene);
}

void TestBattle::onError(cocos2d::extension::AssetsManager::ErrorCode errorCode)
{
    if (errorCode == cocos2d::extension::AssetsManager::ErrorCode::NO_NEW_VERSION)
    {
        CCLOG("no new version");
    }
    else
    {
        m_pScene->onConfigLoadFail("配置文件载入失败，请重启");
    }
}

void TestBattle::onProgress(int percent)
{
    if (percent < 0) return;
    
    m_pScene->m_pLoadingBar->setPercent(percent);
    
    m_pScene->m_pLoadingText->setString(cocos2d::StringUtils::format("%d%%", percent));
}

void TestBattle::onSuccess()
{
    CCLOG("download success");
    m_pScene->onConfigLoadSuccess();
}

cocos2d::Scene* TestBattle::createScene()
{
    m_pScene = TestBattleScene::create();
    
    auto scene = cocos2d::Scene::create();
    scene->addChild(m_pScene);
    
    return scene;
}

cocos2d::ui::Button* TestBattle::createShowPanelButton()
{
    auto* pBtn = cocos2d::ui::Button::create("Assets/TestBattleScene/Button_Normal.png");
    pBtn->setAnchorPoint(cocos2d::Vec2(1, 1));
    pBtn->setTitleColor(cocos2d::Color3B(0, 0, 0));
    pBtn->setContentSize(cocos2d::Size(70, 30));
    pBtn->setTitleText("测试面板");
    pBtn->setPosition(cocos2d::Vec2(1136, 640));
    pBtn->addTouchEventListener(CC_CALLBACK_2(TestBattle::onTouchShowPanelButton, this));
    return pBtn;
}

void TestBattle::onTouchShowPanelButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        m_pScene->setVisible(true);
    }
}

void TestBattle::updateBattleConfig()
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    
    updateTeamConfig(testData.myTeamData, battleFieldConfig.myTeam, m_pScene->m_heroResult);
    updateTeamConfig(testData.enemyTeamData, battleFieldConfig.enemyTeam, m_pScene->m_enemyHeroResult);
}

void TestBattle::updateTeamConfig(TeamData& teamData, BattleConfig::TeamConfig& teamConfig, TestBattleConfigReader::FileNamesResult& heroResult)
{
    if (teamData.heroTestFileName.length() > 0)
    {
        std::string content;
        rapidjson::Document doc;
        TestBattleConfigReader::readTestConfig(teamData.heroTestFileName, heroResult, content);
        doc.Parse<rapidjson::kParseDefaultFlags>(content.c_str());
        
        BattleConfig::getInstance()->parseHeroConfig(*teamConfig.heros[0], doc);
    }
    
    if (teamData.soldierTestFileName.length() > 0)
    {
        std::string content;
        rapidjson::Document doc;
        TestBattleConfigReader::readTestConfig(teamData.soldierTestFileName, m_pScene->m_soldierResult, content);
        doc.Parse<rapidjson::kParseDefaultFlags>(content.c_str());
        
        BattleConfig::getInstance()->parseSoldierConfig(teamConfig.soldier, doc);
    }
    
    if (teamData.formation.length() > 0)
    {
        std::string content;
        TestBattleConfigReader::readTestConfig(teamData.formation, m_pScene->m_formationResult, content);
        teamConfig.formation = content;
    }
    
    if (teamData.ai.length() > 0) teamConfig.heros[0]->heroConfigData.strategy = teamData.ai;
    
    if (teamData.pos.length() > 0)
    {
        if (teamData.pos == "阵前")
        {
            teamConfig.heros[0]->heroConfigData.pos = "f1";
        }
        else if (teamData.pos == "阵后")
        {
            teamConfig.heros[0]->heroConfigData.pos = "b1";
        }
    }
    
    if (teamData.skills.size() > 0)
    {
        teamConfig.heros[0]->clearSkills();
        
        auto numSkills = teamData.skills.size();
        for (int i = 0; i < numSkills; i++)
        {
            auto* pSkillName = teamData.skills[i];
            
            std::string content;
            rapidjson::Document doc;
            TestBattleConfigReader::readTestConfig(*pSkillName, m_pScene->m_skillResult, content);
            doc.Parse<rapidjson::kParseDefaultFlags>(content.c_str());
            
            BattleConfig::getInstance()->parseSkillConfig(*teamConfig.heros[0], doc);
        }
    }
}

void TestBattle::updateBattleConfigFromMonsterJson(std::string& content)
{
    if (m_pScene->m_useMode == TestBattleScene::UseMode::DESIGNER)
    {
        TestBattleConfigReader::saveTempFile("def.txt", content);
        
        rapidjson::Document fromData;
        fromData.Parse<rapidjson::kParseDefaultFlags>(content.c_str());
        
        rapidjson::Document docOutput;
        rapidjson::Value toData(rapidjson::kObjectType);
        createBattleConfigData(toData, fromData, docOutput, nullptr);
        
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        toData.Accept(writer);
        
        content = std::string(buffer.GetString());
        
        TestBattleConfigReader::saveTempFile("abc.txt", content);
    }
}

void TestBattle::createBattleConfigData(rapidjson::Value& toData, rapidjson::Value& fromData, rapidjson::Document& doc, rapidjson::Value* pJSON)
{
    auto& allocator = doc.GetAllocator();
    
    if (fromData.IsObject())
    {
        for (auto iter = fromData.MemberonBegin(); iter != fromData.MemberonEnd(); iter++)
        {
            std::string key = iter->name.GetString();
            
            if (iter->value.IsObject())
            {
                rapidjson::Value valName(key.c_str(), allocator);
                rapidjson::Value valObj(rapidjson::kObjectType);
                bool memberValid = true;
                
                if (key == "myTeam")
                {
                    createBattleConfigData(valObj, iter->value, doc, testData.myTeamData.gd_heroJSON);
                }
                else if (key == "enemyTeam")
                {
                    createBattleConfigData(valObj, iter->value, doc, testData.enemyTeamData.gd_heroJSON);
                }
                else if (key == "soldier")
                {
                    if (pJSON && pJSON->HasMember("soldier") && !(*pJSON)["soldier"].IsNull())
                    {
                        createBattleConfigData(valObj, iter->value, doc, &(*pJSON)["soldier"]);
                    }
                    else
                    {
                        memberValid = false;
                    }
                }
                else if (key == "hero")
                {
                    createBattleConfigData(valObj, iter->value, doc, pJSON);
                }
                else if (key == "base")
                {
                    createBattleConfigData(valObj, iter->value, doc, pJSON);
                }
                else
                {
                    createBattleConfigData(valObj, iter->value, doc, nullptr);
                }
                
                if (memberValid)
                {
                    toData.AddMember(valName, valObj, allocator);
                }
            }
            else if (iter->value.IsArray())
            {
                rapidjson::Value valName(key.c_str(), allocator);
                rapidjson::Value valArray(rapidjson::kArrayType);
                
                if (key == "skills")
                {
                    if (pJSON && pJSON->HasMember("skills") && iter->value.Size() > 0)
                    {
                        rapidjson::Value& jsonSkills = (*pJSON)["skills"];
                        for (int i = 0; i < jsonSkills.Size(); i++)
                        {
                            rapidjson::Value valObj(rapidjson::kObjectType);

                            createBattleConfigData(valObj, iter->value[0u], doc, &jsonSkills[i]);
                            
                            valArray.PushBack(valObj, allocator);
                        }
                    }
                }
                
                toData.AddMember(valName, valArray, allocator);
            }
            else
            {
                std::string jsonKey = key;
                if (key == "soldierType") jsonKey = "type";
                else if (key == "config") jsonKey = "mark";
                else if (key == "mpCost") jsonKey = "MPCost";
                else if (key == "pos") jsonKey = "position";
                
                if (pJSON && pJSON->HasMember(jsonKey.c_str()))
                {
                    addMemberToData(toData, iter->value, key, (*pJSON)[jsonKey.c_str()], doc);
                }
                else
                {
                    addMemberToData(toData, iter->value, key, iter->value, doc);
                }
            }
        }
    }
}

void TestBattle::addMemberToData(rapidjson::Value& toData, rapidjson::Value& fromValue, const std::string& key, rapidjson::Value& value, rapidjson::Document& doc)
{
    std::string stream("");
    rapidjson::Value& finalValue = (value.IsNull() ? fromValue : value);
    if (finalValue.IsString())
    {
        stream = finalValue.GetString();
    }
    else if (finalValue.IsInt())
    {
        stream = cocos2d::StringUtils::format("%d", finalValue.GetInt());
    }
    else if (finalValue.IsDouble())
    {
        stream = cocos2d::StringUtils::format("%f", finalValue.GetDouble());
    }
    
    auto& allocator = doc.GetAllocator();
    
    rapidjson::Value valName(key.c_str(), allocator);
    
    if (fromValue.IsString())
    {
        rapidjson::Value valValue(stream.c_str(), allocator);
        
        toData.AddMember(valName, valValue, allocator);
    }
    else if (fromValue.IsNumber())
    {
        if (finalValue.IsInt())
        {
            int i = atoi(stream.c_str());
            rapidjson::Value valValue(i);
            
            toData.AddMember(valName, valValue, allocator);
        }
        else
        {
            double d = atof(stream.c_str());
            rapidjson::Value valValue(d);
            
            toData.AddMember(valName, valValue, allocator);
        }
    }
}


void TestBattle::startBattle()
{
    BattleWorld::getInstance()->setAutoQuickBattleMode(false);
    
    std::string content;
    TestBattleConfigReader::readTestConfig(testData.fightName, m_pScene->m_fightResult, content);
    
    updateBattleConfigFromMonsterJson(content);
    
    BattleConfig::getInstance()->initBattleFieldConfig(content);
    
    updateBattleConfig();
    
    auto battle = BattleSG::create();
    battle->setName("battleLayer");
    
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->addChild(battle, -1);
    
    m_pScene->setVisible(false);
}

void TestBattle::endBattle()
{
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    scene->removeChildByName("battleLayer");
}

void TestBattle::loadVersion()
{
    std::string configDir = cocos2d::FileUtils::getInstance()->getWritablePath() + "config/";
    if (cocos2d::FileUtils::getInstance()->isDirectoryExist(configDir))
    {
        if (!cocos2d::FileUtils::getInstance()->removeDirectory(configDir))
        {
            m_pScene->onConfigLoadFail("删除旧配置失败，请重启");
            return;
        }
    }
    
    std::string url = cocos2d::StringUtils::format("%slatest?rnd=%f", m_configPath.c_str(), cocos2d::random<float>(0, 100));
    
    cocos2d::network::HttpRequest* pRequest = new cocos2d::network::HttpRequest();
    pRequest->setUrl(url.c_str());
    pRequest->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    pRequest->setResponseCallback(CC_CALLBACK_2(TestBattle::onLoadVersionCompleted, this));
    
    cocos2d::network::HttpClient::getInstance()->send(pRequest);
    
    pRequest->release();
}

void TestBattle::onLoadVersionCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (response && response->isSucceed())
    {
        auto* buffer = response->getResponseData();
        std::string str(buffer->begin(), buffer->end());
        str = str.substr(0, str.find("\n"));
        
        m_version = str;
        
        std::string configURL = cocos2d::StringUtils::format("%s%s/config.zip?rnd=%f", m_configPath.c_str(), m_version.c_str(), cocos2d::random<float>(0, 100));
        std::string verURL = cocos2d::StringUtils::format("%slatest?rnd=%f", m_configPath.c_str(), cocos2d::random<float>(0, 100));
        
        CCLOG("config url %s", configURL.c_str());
        CCLOG("ver url %s", verURL.c_str());
        
        m_pAssetsManager = new cocos2d::extension::AssetsManager(configURL.c_str(), verURL.c_str(), cocos2d::FileUtils::getInstance()->getWritablePath().c_str());
        m_pAssetsManager->setDelegate(this);
        m_pAssetsManager->setConnectionTimeout(8);
        m_pAssetsManager->deleteVersion();
        
        if (m_pAssetsManager->checkUpdate())
        {
            CCLOG("config need to update");
            m_pScene->onConfigLoadBegin();
            
            m_pAssetsManager->update();
        }
        else
        {
            CCLOG("config no need to update");
            m_pScene->onConfigLoadSuccess();
        }
    }
    else
    {
        m_pScene->onVersionLoadFail();
    }
}

#pragma mark -
#pragma mark TestBattleScene

TestBattleScene::TestBattleScene()
{
    Size frameSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    m_pLayout = CSLoader::getInstance()->createNode("Scene/TestBattleScene.csb");
    m_pLayout->setContentSize(frameSize);
    cocos2d::ui::Helper::doLayout(m_pLayout);
    this->addChild(m_pLayout);
    
    auto* pPanelWelcome = m_pLayout->getChildByName("Panel_Welcome");
    pPanelWelcome->getChildByName<cocos2d::ui::Button*>("Button_No")->addTouchEventListener(CC_CALLBACK_2(TestBattleScene::onTouchNotToLoadConfig, this));
    pPanelWelcome->getChildByName<cocos2d::ui::Button*>("Button_Yes")->addTouchEventListener(CC_CALLBACK_2(TestBattleScene::onTouchYesToLoadConfig, this));
    
    auto* pPanelProgress = m_pLayout->getChildByName("Panel_Progress");
    pPanelProgress->setVisible(false);
    m_pLoadingBar = pPanelProgress->getChildByName<cocos2d::ui::LoadingBar*>("LoadingBar_Progress");
    m_pLoadingText = pPanelProgress->getChildByName<cocos2d::ui::Text*>("Text_Progress");
    m_pLoadingTip = pPanelProgress->getChildByName<cocos2d::ui::Text*>("Text_8");
    
    auto* pPanelSelect = m_pLayout->getChildByName("Panel_Select");
    pPanelSelect->setVisible(false);
    m_pTextMyDetail = pPanelSelect->getChildByName<cocos2d::ui::Text*>("Text_Detail_My");
    m_pTextEnemyDetail = pPanelSelect->getChildByName<cocos2d::ui::Text*>("Text_Detail_Enemy");
    m_pTextPage = pPanelSelect->getChildByName<cocos2d::ui::Text*>("Text_Page");
    m_pTextFight = pPanelSelect->getChildByName<cocos2d::ui::Text*>("Text_Fight");
    m_pBtnPre = pPanelSelect->getChildByName<cocos2d::ui::Button*>("Button_Pre");
    m_pBtnNext = pPanelSelect->getChildByName<cocos2d::ui::Button*>("Button_Next");
    
    auto* pListView = pPanelSelect->getChildByName<cocos2d::ui::ListView*>("ListView_Configs");
    auto numItems = pListView->getItems().size();
    for (int i = 0; i < numItems; i++)
    {
        auto* pNewItem = dynamic_cast<cocos2d::ui::ListView*>(pListView->getItem(i));
        if (pNewItem)
        {
            pNewItem->addEventListener((cocos2d::ui::ListView::ccListViewCallback)CC_CALLBACK_2(TestBattleScene::onSelectListViewItem, this));
        }
    }
    m_pListViewFight = static_cast<cocos2d::ui::ListView*>(pListView->getItem(0));
    m_pListViewHero = static_cast<cocos2d::ui::ListView*>(pListView->getItem(1));
    m_pListViewPos = static_cast<cocos2d::ui::ListView*>(pListView->getItem(2));
    m_pListViewSoldier = static_cast<cocos2d::ui::ListView*>(pListView->getItem(3));
    m_pListViewFormation = static_cast<cocos2d::ui::ListView*>(pListView->getItem(4));
    m_pListViewSkill = static_cast<cocos2d::ui::ListView*>(pListView->getItem(6));
    m_pListViewAI = static_cast<cocos2d::ui::ListView*>(pListView->getItem(7));
    
    auto* pGDPanel = pPanelSelect->getChildByName("Panel_GD");
    for (int i = 0; i < 4; i++)
    {
        auto* pBtn = pGDPanel->getChildByName<cocos2d::ui::Button*>(cocos2d::StringUtils::format("Button_GD_%d", i));
        pBtn->addTouchEventListener(CC_CALLBACK_2(TestBattleScene::onTouchGDFilterClearButton, this));
    }
    for (int i = 0; i < 7; i++)
    {
        m_gdTexts.push_back(pGDPanel->getChildByName<cocos2d::ui::Text*>(cocos2d::StringUtils::format("Text_GD_%d", i)));
    }
    
    pListView = pGDPanel->getChildByName<cocos2d::ui::ListView*>("ListView_GD");
    numItems = pListView->getItems().size();
    for (int i = 0; i < numItems; i++)
    {
        auto* pNewItem = dynamic_cast<cocos2d::ui::ListView*>(pListView->getItem(i));
        if (pNewItem)
        {
            pNewItem->addEventListener((cocos2d::ui::ListView::ccListViewCallback)CC_CALLBACK_2(TestBattleScene::onSelectListViewItem, this));
        }
    }
    m_pListViewGDHeroName = static_cast<cocos2d::ui::ListView*>(pListView->getItem(0));
    m_pListViewGDHeroLevel = static_cast<cocos2d::ui::ListView*>(pListView->getItem(1));
    m_pListViewGDHeroSubType = static_cast<cocos2d::ui::ListView*>(pListView->getItem(2));
    m_pListViewGDHero = static_cast<cocos2d::ui::ListView*>(pListView->getItem(4));
    m_pListViewGDFormation = static_cast<cocos2d::ui::ListView*>(pListView->getItem(5));
    m_pListViewGDPos = static_cast<cocos2d::ui::ListView*>(pListView->getItem(6));
    m_pListViewGDAI = static_cast<cocos2d::ui::ListView*>(pListView->getItem(7));
    
    m_pBtnPre->addTouchEventListener(CC_CALLBACK_2(TestBattleScene::onTouchPageButton, this));
    m_pBtnNext->addTouchEventListener(CC_CALLBACK_2(TestBattleScene::onTouchPageButton, this));
}

void TestBattleScene::onExit()
{
    Layer::onExit();
}

void TestBattleScene::onVersionLoadFail()
{
    m_pLayout->getChildByName("Panel_Progress")->setVisible(true);
    m_pLoadingBar->setPercent(0);
    m_pLoadingText->setString("0%");
    m_pLoadingTip->setString("版本号载入失败，请重启");
}

void TestBattleScene::onConfigLoadBegin()
{
    m_pLayout->getChildByName("Panel_Progress")->setVisible(true);
    m_pLoadingBar->setPercent(0);
    m_pLoadingText->setString("0%");
    m_pLoadingTip->setString("配置更新中...");
}

void TestBattleScene::onConfigLoadFail(const std::string& msg)
{
    m_pLoadingTip->setString(msg);
}

void TestBattleScene::onConfigLoadSuccess()
{
    m_pLayout->getChildByName("Panel_Progress")->setVisible(false);
    m_pLayout->getChildByName("Panel_Select")->setVisible(true);
    
    m_pLayout->getChildByName("Panel_Select")->getChildByName<cocos2d::ui::Text*>("Text_Ver")->setString(TestBattle::getInstance()->m_version);
    
    initData();
}

void TestBattleScene::initData()
{
    if (TestBattle::getInstance()->m_pAssetsManager)
    {
        CCLOG("latest ver %s", TestBattle::getInstance()->m_pAssetsManager->getVersion().c_str());
        
        TestBattleConfigReader::autoGenerateSkillTestConfig();
    }
    
    TestBattleConfigReader::filenamesAtFolder("config/test/fight", "json", m_fightResult);
    
    m_posResult.filenames.push_back(new std::string("阵前"));
    m_posResult.filenames.push_back(new std::string("阵后"));
    
    if (m_pLayout->getChildByName("Panel_Welcome")->getChildByName<cocos2d::ui::CheckBox*>("CheckBox_GD")->isSelected())
    {
        m_useMode = UseMode::DESIGNER;
        
        TestBattle::getInstance()->testData.fightName = "gd";
        
        m_pLayout->getChildByName("Panel_Select")->getChildByName("ListView_Configs")->setVisible(false);
        m_pLayout->getChildByName("Panel_Select")->getChildByName("Panel_GD")->setVisible(true);
        
        TestBattleConfigReader::readMonsterJSON(m_monsterResult);
        
        TestBattleConfigReader::filenamesAtFolder("config/test/formation", "txt", m_formationResult);
        TestBattleConfigReader::filenamesAtFolder("config/ai", "json", m_aiResult);
        
        initListView(m_pListViewGDFormation, m_formationResult);
        initListView(m_pListViewGDPos, m_posResult);
        initListView(m_pListViewGDAI, m_aiResult);
    }
    else
    {
        m_useMode = UseMode::TESTER;
        
        TestBattle::getInstance()->testData.fightName = "default";
        
        m_pLayout->getChildByName("Panel_Select")->getChildByName("ListView_Configs")->setVisible(true);
        m_pLayout->getChildByName("Panel_Select")->getChildByName("Panel_GD")->setVisible(false);
        
        TestBattleConfigReader::filenamesAtFolder("config/test/hero", "json", m_heroResult);
        TestBattleConfigReader::filenamesAtFolder("config/test/hero_enemy", "json", m_enemyHeroResult);
        TestBattleConfigReader::filenamesAtFolder("config/test/soldier", "json", m_soldierResult);
        TestBattleConfigReader::filenamesAtFolder("config/test/formation", "txt", m_formationResult);
        TestBattleConfigReader::filenamesAtFolder("config/test/skill", "json", m_skillResult);
        TestBattleConfigReader::filenamesAtFolder("config/ai", "json", m_aiResult);
        
        initListView(m_pListViewFight, m_fightResult);
        initListView(m_pListViewHero, m_heroResult);
        initListView(m_pListViewFormation, m_formationResult);
        initListView(m_pListViewSkill, m_skillResult);
        initListView(m_pListViewSoldier, m_soldierResult);
        initListView(m_pListViewAI, m_aiResult);
        initListView(m_pListViewPos, m_posResult);
    }
    
    updateUI();
}

void TestBattleScene::initListView(cocos2d::ui::ListView* pListView, TestBattleConfigReader::FileNamesResult& result)
{
    pListView->setItemModel(pListView->getItem(0));
    pListView->removeAllItems();
    
    auto numFiles = result.filenames.size();
    for (int i = 0; i < numFiles; i++)
    {
        pListView->pushBackDefaultItem();
        
        auto* pBtn = static_cast<cocos2d::ui::Button*>(pListView->getItem(i));
        pBtn->setTitleText(*result.filenames[i]);
    }
}

void TestBattleScene::initGDListView(cocos2d::ui::ListView* pListView, std::vector<std::string>& values)
{
    pListView->setItemModel(pListView->getItem(0));
    pListView->removeAllItems();
    
    auto num = values.size();
    for (int i = 0; i < num; i++)
    {
        pListView->pushBackDefaultItem();
        
        auto* pBtn = static_cast<cocos2d::ui::Button*>(pListView->getItem(i));
        pBtn->setTitleText(values[i]);
    }
}

void TestBattleScene::onTouchYesToLoadConfig(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        m_pLayout->getChildByName("Panel_Welcome")->setVisible(false);
        m_pLayout->getChildByName("Panel_Progress")->setVisible(true);
        m_pLayout->getChildByName("Panel_Select")->setVisible(false);

        TestBattle::getInstance()->loadVersion();
    }
}

void TestBattleScene::onTouchNotToLoadConfig(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        m_pLayout->getChildByName("Panel_Welcome")->setVisible(false);
        m_pLayout->getChildByName("Panel_Progress")->setVisible(false);
        m_pLayout->getChildByName("Panel_Select")->setVisible(true);
        
        onConfigLoadSuccess();
    }
}

void TestBattleScene::onTouchPageButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        auto* pBtn = static_cast<cocos2d::ui::Button*>(pSender);
        if (pBtn == m_pBtnPre)
        {
            if (m_currentStep == Step::CONFIG_MY_TEAM)
            {
                changeStep(Step::START_BATTLE);
            }
            else if (m_currentStep == Step::CONFIG_ENEMY_TEAM)
            {
                changeStep(Step::CONFIG_MY_TEAM);
            }
            else if (m_currentStep == Step::START_BATTLE)
            {
                TestBattle::getInstance()->m_pScene->setVisible(false);
            }
        }
        else if (pBtn == m_pBtnNext)
        {
            if (m_currentStep == Step::CONFIG_MY_TEAM)
            {
                changeStep(Step::CONFIG_ENEMY_TEAM);
            }
            else if (m_currentStep == Step::CONFIG_ENEMY_TEAM)
            {
                changeStep(Step::START_BATTLE);
            }
            else if (m_currentStep == Step::START_BATTLE)
            {
                changeStep(Step::CONFIG_MY_TEAM);
            }
        }
    }
}

void TestBattleScene::changeStep(Step newStep)
{
    Step oldStep = m_currentStep;
    m_currentStep = newStep;
    
    if (newStep == Step::START_BATTLE)
    {
        updateUI();
        TestBattle::getInstance()->startBattle();
    }
    else if (newStep == Step::CONFIG_MY_TEAM)
    {
        if (oldStep == Step::START_BATTLE)
        {
            TestBattle::getInstance()->endBattle();
        }
        
        if (m_useMode == UseMode::TESTER)
        {
            initListView(m_pListViewHero, m_heroResult);
        }
        else if (m_useMode == UseMode::DESIGNER)
        {
            m_selectedName = "";
            m_selectedLevel = "";
            m_selectedSubType = "";
            m_monsterResult.resetFilter();
            
            m_selectedFormation = "";
            m_selectedPos = "";
            m_selectedAI = "";
        }
        
        updateUI();
    }
    else if (newStep == Step::CONFIG_ENEMY_TEAM)
    {
        if (m_useMode == UseMode::TESTER)
        {
            initListView(m_pListViewHero, m_enemyHeroResult);
        }
        else if (m_useMode == UseMode::DESIGNER)
        {
            m_selectedName = "";
            m_selectedLevel = "";
            m_selectedSubType = "";
            m_monsterResult.resetFilter();
            
            m_selectedFormation = "";
            m_selectedPos = "";
            m_selectedAI = "";
        }
        
        updateUI();
    }
}

void TestBattleScene::onSelectListViewItem(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
{
    if (type == cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END)
    {
        if (m_currentStep != Step::CONFIG_MY_TEAM && m_currentStep != Step::CONFIG_ENEMY_TEAM) return;
        
        auto* pListView = static_cast<cocos2d::ui::ListView*>(pSender);
        
        auto idx = pListView->getCurSelectedIndex();
        auto& testData = TestBattle::getInstance()->testData;
        auto& teamData = (m_currentStep == Step::CONFIG_MY_TEAM ? testData.myTeamData : testData.enemyTeamData);
        
        if (pListView == m_pListViewFight)
        {
            testData = TestBattle::TestData();
            testData.fightName = *m_fightResult.filenames[idx];
        }
        else if (pListView == m_pListViewHero)
        {
            if (m_currentStep == Step::CONFIG_MY_TEAM)
            {
                teamData.heroTestFileName = *m_heroResult.filenames[idx];
            }
            else if (m_currentStep == Step::CONFIG_ENEMY_TEAM)
            {
                teamData.heroTestFileName = *m_enemyHeroResult.filenames[idx];
            }
        }
        else if (pListView == m_pListViewFormation)
        {
            teamData.formation = *m_formationResult.filenames[idx];
        }
        else if (pListView == m_pListViewGDFormation)
        {
            teamData.formation = *m_formationResult.filenames[idx];
            m_selectedFormation = teamData.formation;
        }
        else if (pListView == m_pListViewSkill)
        {
            if (teamData.skills.size() >= 4) teamData.clearSkills();
            teamData.skills.push_back(new std::string(*m_skillResult.filenames[idx]));
        }
        else if (pListView == m_pListViewSoldier)
        {
            teamData.soldierTestFileName = *m_soldierResult.filenames[idx];
        }
        else if (pListView == m_pListViewAI)
        {
            teamData.ai = *m_aiResult.filenames[idx];
        }
        else if (pListView == m_pListViewGDAI)
        {
            teamData.ai = *m_aiResult.filenames[idx];
            m_selectedAI = teamData.ai;
        }
        else if (pListView == m_pListViewPos)
        {
            teamData.pos = *m_posResult.filenames[idx];
        }
        else if (pListView == m_pListViewGDPos)
        {
            teamData.pos = *m_posResult.filenames[idx];
            m_selectedPos = teamData.pos;
        }
        else if (pListView == m_pListViewGDHeroName)
        {
            m_selectedName = m_monsterNames[idx];
            m_monsterResult.filterMonsters("name", m_selectedName);
        }
        else if (pListView == m_pListViewGDHeroLevel)
        {
            m_selectedLevel = m_monsterLevels[idx];
            m_monsterResult.filterMonsters("level", m_selectedLevel);
        }
        else if (pListView == m_pListViewGDHeroSubType)
        {
            m_selectedSubType = m_monsterSubTypes[idx];
            m_monsterResult.filterMonsters("monsterSubType", m_selectedSubType);
        }
        else
        {
            return;
        }
        
        if (m_monsterResult.monstersFiltered.size() == 1)
        {
            teamData.gd_heroJSON = m_monsterResult.monstersFiltered[0];
        }
        else
        {
            teamData.gd_heroJSON = nullptr;
        }
        
        scheduleOnce(schedule_selector(TestBattleScene::updateUIDelay), 0.1);
    }
}

void TestBattleScene::onTouchGDFilterClearButton(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        auto* pBtn = static_cast<cocos2d::ui::Button*>(pSender);
        if (pBtn->getName() == "Button_GD_0")
        {
            m_selectedName = "";
            
            m_monsterResult.resetFilter();
            m_monsterResult.filterMonsters("name", m_selectedName);
            m_monsterResult.filterMonsters("level", m_selectedLevel);
            m_monsterResult.filterMonsters("monsterSubType", m_selectedSubType);
        }
        else if (pBtn->getName() == "Button_GD_1")
        {
            m_selectedLevel = "";
            
            m_monsterResult.resetFilter();
            m_monsterResult.filterMonsters("name", m_selectedName);
            m_monsterResult.filterMonsters("level", m_selectedLevel);
            m_monsterResult.filterMonsters("monsterSubType", m_selectedSubType);
        }
        else if (pBtn->getName() == "Button_GD_2")
        {
            m_selectedSubType = "";
            
            m_monsterResult.resetFilter();
            m_monsterResult.filterMonsters("name", m_selectedName);
            m_monsterResult.filterMonsters("level", m_selectedLevel);
            m_monsterResult.filterMonsters("monsterSubType", m_selectedSubType);
        }
        else if (pBtn->getName() == "Button_GD_3")
        {
            m_selectedName = "";
            m_selectedLevel = "";
            m_selectedSubType = "";
            
            m_monsterResult.resetFilter();
        }
        
        updateUI();
    }
}

void TestBattleScene::updateUIDelay(float dt)
{
    updateUI();
}

void TestBattleScene::updateUI()
{
    if (m_currentStep == Step::CONFIG_MY_TEAM)
    {
        m_pBtnPre->setVisible(true);
        m_pBtnNext->setVisible(true);
        m_pBtnPre->setTitleText("开战");
        m_pBtnNext->setTitleText("配置敌方");
        
        m_pBtnPre->setVisible(true);
        if (m_useMode == UseMode::DESIGNER)
        {
            auto& testData = TestBattle::getInstance()->testData;
            if (testData.myTeamData.gd_heroJSON && testData.myTeamData.formation.length() > 0
                && testData.enemyTeamData.gd_heroJSON && testData.enemyTeamData.formation.length() > 0 && testData.enemyTeamData.ai.length() > 0)
            {
                m_pBtnPre->setVisible(true);
            }
            else
            {
                m_pBtnPre->setVisible(false);
            }
        }
    }
    else if (m_currentStep == Step::CONFIG_ENEMY_TEAM)
    {
        m_pBtnPre->setVisible(true);
        m_pBtnNext->setVisible(true);
        m_pBtnPre->setTitleText("配置己方");
        m_pBtnNext->setTitleText("开战");
        
        m_pBtnNext->setVisible(true);
        if (m_useMode == UseMode::DESIGNER)
        {
            auto& testData = TestBattle::getInstance()->testData;
            if (testData.myTeamData.gd_heroJSON && testData.myTeamData.formation.length() > 0
                && testData.enemyTeamData.gd_heroJSON && testData.enemyTeamData.formation.length() > 0 && testData.enemyTeamData.ai.length() > 0)
            {
                m_pBtnNext->setVisible(true);
            }
            else
            {
                m_pBtnNext->setVisible(false);
            }
        }
    }
    else if (m_currentStep == Step::START_BATTLE)
    {
        m_pBtnPre->setVisible(true);
        m_pBtnNext->setVisible(true);
        m_pBtnPre->setTitleText("继续战斗");
        m_pBtnNext->setTitleText("结束战斗");
    }
    
    if (m_useMode == UseMode::DESIGNER)
    {
        m_monsterResult.enumValues("name", m_monsterNames);
        m_monsterResult.enumValues("level", m_monsterLevels);
        m_monsterResult.enumValues("monsterSubType", m_monsterSubTypes);
        m_monsterResult.enumValues("name", m_monsterNamesFiltered);
        
        initGDListView(m_pListViewGDHeroName, m_monsterNames);
        initGDListView(m_pListViewGDHeroLevel, m_monsterLevels);
        initGDListView(m_pListViewGDHeroSubType, m_monsterSubTypes);
        initGDListView(m_pListViewGDHero, m_monsterNamesFiltered);
        
        m_gdTexts[0]->setString("武将:" + m_selectedName);
        m_gdTexts[1]->setString("等级:" + m_selectedLevel);
        m_gdTexts[2]->setString("标签:" + m_selectedSubType);
        m_gdTexts[4]->setString(m_selectedFormation);
        m_gdTexts[5]->setString(m_selectedPos);
        m_gdTexts[6]->setString(m_selectedAI);
        
        if (m_monsterResult.monstersFiltered.size() == 1)
        {
            m_gdTexts[3]->setString((*m_monsterResult.monstersFiltered[0])["name"].GetString());
        }
        else
        {
            m_gdTexts[3]->setString("");
        }
    }
    
    const auto& testData = TestBattle::getInstance()->testData;
    m_pTextFight->setString(testData.fightName);
    updateTextByTeamData(m_pTextMyDetail, testData.myTeamData);
    updateTextByTeamData(m_pTextEnemyDetail, testData.enemyTeamData);
}

void TestBattleScene::updateTextByTeamData(cocos2d::ui::Text* pText, const TestBattle::TeamData& teamData)
{
    std::string textStr("");
    
    if (m_useMode == UseMode::TESTER)
    {
        textStr += (teamData.heroTestFileName.length() > 0 ? teamData.heroTestFileName : "英雄无");
        textStr += ",";
        textStr += (teamData.soldierTestFileName.length() > 0 ? teamData.soldierTestFileName : "兵种无");
        textStr += ",";
        textStr += (teamData.formation.length() > 0 ? teamData.formation : "阵型无");
        textStr += ",";
        textStr += (teamData.pos.length() > 0 ? teamData.pos : "位置无");
        textStr += ",";
        textStr += (teamData.ai.length() > 0 ? teamData.ai : "AI无");
        textStr += ",\n";
        textStr += (teamData.skills.size() > 0 ? *teamData.skills[0] : "技能1无");
        textStr += ",";
        textStr += (teamData.skills.size() > 1 ? *teamData.skills[1] : "技能2无");
        textStr += ",";
        textStr += (teamData.skills.size() > 2 ? *teamData.skills[2] : "技能3无");
        textStr += ",";
        textStr += (teamData.skills.size() > 3 ? *teamData.skills[3] : "技能4无");
    }
    else if (m_useMode == UseMode::DESIGNER)
    {
        if (teamData.gd_heroJSON)
        {
            auto& jsonData = *teamData.gd_heroJSON;
            textStr += std::string(jsonData["name"].GetString()) + ",";
            textStr += "id:" + std::string(jsonData["id"].GetString()) + ",";
            textStr += "lv:" + std::string(jsonData["level"].GetString()) + ",";
            textStr += "HP:" + cocos2d::StringUtils::format("%f", jsonData["HP"].GetDouble()) + ",";
            textStr += "MP:" + cocos2d::StringUtils::format("%f", jsonData["MP"].GetDouble()) + ",";
            textStr += "atk:" + cocos2d::StringUtils::format("%f", jsonData["attack"].GetDouble()) + ",";
            textStr += teamData.formation + ",";
            textStr += teamData.pos + ",";
            textStr += teamData.ai + ", ";
            
            if (jsonData.HasMember("soldier"))
            {
                textStr += std::string(jsonData["soldier"]["name"].GetString()) + ",";
                textStr += "id:" + cocos2d::StringUtils::format("%f", jsonData["soldier"]["id"].GetDouble()) + ",";
                textStr += "num:" + cocos2d::StringUtils::format("%f", jsonData["soldier"]["num"].GetDouble()) + ",";
                textStr += "HP:" + cocos2d::StringUtils::format("%f", jsonData["soldier"]["HP"].GetDouble()) + ",";
                textStr += "atk:" + cocos2d::StringUtils::format("%f", jsonData["soldier"]["attack"].GetDouble()) + ", ";
            }
            
            if (jsonData.HasMember("skills"))
            {
                auto& jsonSkills = jsonData["skills"];
                for (int i = 0; i < jsonSkills.Size(); i++)
                {
                    auto& jsonSkill = jsonSkills[i];
                    textStr += std::string(jsonSkill["mark"].GetString()) + ",";
                    textStr += "lv:" + std::string(jsonSkill["level"].GetString()) + ", ";
                }
            }
        }
        else
        {
            textStr += "英雄无";
        }
    }
    
    pText->setString(textStr);
}

#endif
