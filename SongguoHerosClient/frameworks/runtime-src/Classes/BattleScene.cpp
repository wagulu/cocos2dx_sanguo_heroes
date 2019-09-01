
#include "battle/BattleConfig.h"
#include "BattleScene.h"
#include "BattleWorld.h"
#include "AudioManager.h"
#include "SkillManager.h"
#include "BattleAudioConfig.h"

extern "C"
{
#include "CCLuaEngine.h"
#include "lua.h"
#include "tolua++.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

USING_NS_CC;

int createBattle(lua_State *L)
{
    const char* str = lua_tostring(L, 1);
    BattleConfig::getInstance()->initBattleFieldConfig(std::string(str));

    bool autoQuickBattleMode = lua_toboolean(L, 2);
    BattleWorld::getInstance()->setAutoQuickBattleMode(autoQuickBattleMode);
    
    auto battleLayer = BattleSG::create();
    object_to_luaval<cocos2d::Layer>(L, "cc.Layer", (cocos2d::Layer*)battleLayer);
    
    return 1;
}

Scene* BattleSG::createScene()
{
    BattleConfig::getInstance()->initBattleFieldConfig(cocos2d::FileUtils::getInstance()->getStringFromFile("config/fight.json"));
    
    BattleWorld::getInstance()->setAutoQuickBattleMode(false);
    
    auto battleLayer = BattleSG::create();

    auto scene = Scene::create();
    scene->addChild(battleLayer);
    
    return scene;
}

BattleSG::BattleSG()
{
    BattleAudioConfig::getInstance()->initConfig("sound/battleConfig.json");
    SkillManager::getInstance()->initBuffEffect("config/buffEffect.json");
    
    m_battleLoadingUI = BattleLoadingUI::create();
    m_battleLoadingUI->setName("loadingPanel");
    this->addChild(m_battleLoadingUI);
    
    scheduleOnce(schedule_selector(BattleSG::createBattleWorld), 0.1);
}

void BattleSG::createBattleWorld(float dt)
{
    BattleWorld::getInstance()->create();
    this->addChild(_BATTLE_STAGE.getStage());
    
    _ENTITY_EVENT.subscribe<BattleEvent::StartBattleProgress>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::StartBattleComplete>(*this);
}

BattleSG::~BattleSG()
{
    BattleWorld::getInstance()->destory();
}

void BattleSG::receive(const BattleEvent::StartBattleProgress& e)
{
    if (m_battleLoadingUI)
    {
        m_battleLoadingUI->updateProgress(e.percent);
    }
}

void BattleSG::receive(const BattleEvent::StartBattleComplete& e)
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::StartBattleProgress>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::StartBattleComplete>(*this);
    
    this->getChildByName("loadingPanel")->removeFromParent();
}
