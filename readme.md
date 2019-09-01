# Ccocos2dx c++ & lua 

## Game Video

### [youtobe](https://www.youtube.com/watch?v=h7zm4OyMdSo&feature=youtu.be)

## Client

### version:cocos2dx 3.6 (c++&lua) 
### gameplay:c++ 
### ui logic:lua
### c/s protcol: protobuf(pbc lua) 
### design pattern ECS [entityx](https://github.com/alecthomas/entityx.git) 

## How to Use

### run with mac

#### install |CocosFramework-v3.6.pkg|[click download](http://www.cocos2d-x.org/filedown/CocosFramework-v3.6.pkg)|

#### open **SanguoClient/frameworks⁩/⁨runtime-src⁩/proj.ios_mac⁩/sanguoGame.xcodeproj** by Xcode


## Battle images

[![Promo](/images/gif/0.gif)]

[![Promo](/images/gif/1.gif)]

[![Promo](/images/gif/2.gif)]

[![Promo](/images/gif/3.gif)]

## Code Path

### [c++ code path](SanguoClient/frameworks/runtime-src/Classes/)⁩
### [lua code path](SanguoClient/src)⁩

## Example code

### cpp

```cpp 

#include "SkillSystem.h"
#include "BattleWorld.h"
#include "AudioManager.h"
#include "AICommonHelper.h"

SkillSystem::~SkillSystem()
{
}

void SkillSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::FireSkill>(*this);
    events.subscribe<BattleEvent::FireNegativeSkill>(*this);
    events.subscribe<BattleEvent::CancelSkill>(*this);
    events.subscribe<BattleEvent::TriggerSkillAction>(*this);
    events.subscribe<BattleEvent::BattlePause>(*this);
    events.subscribe<BattleEvent::BattleResume>(*this);
}

void SkillSystem::receive(const BattleEvent::BattlePause& e)
{
    m_isPausing = true;
}

void SkillSystem::receive(const BattleEvent::BattleResume& e)
{
    m_isPausing = false;
}

void SkillSystem::receive(const BattleEvent::FireSkill& e)
{
    auto entity = _ENTITY_MANAGER.create();
    auto* skillObj = SkillManager::getInstance()->createSkill(e.pSkillData, e.fromId, entity.id());
    entity.assign<BattleComponent::Skill>(skillObj, e.fromId);
    
    _CAMERA_MANAGER.setCameraCauseID(entity.id().id());
}

void SkillSystem::receive(const BattleEvent::FireNegativeSkill& e)
{
    auto entity = _ENTITY_MANAGER.create();
    auto* skillObj = SkillManager::getInstance()->createSkill(e.pSkillData, e.fromId, entity.id());
    entity.assign<BattleComponent::Skill>(skillObj, e.fromId);
}

void SkillSystem::receive(const BattleEvent::CancelSkill& e)
{
    auto identify = e.entity.component<BattleComponent::Identify>();
    
    BattleComponent::Skill::Handle skill;
    for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(skill))
    {
        if (LEFT_HERO == identify->id)
        {
            _CAMERA_MANAGER.setCameraUserControlLocked(false);
        }
        
        bool breakSucceed = false;
        if (skill->fromId == identify->id)
        {
            auto* pSkillData = skill->skill->getSkillData();
            if (e.forceCancel)
            {
                breakSucceed = true;
            }
            else if (pSkillData->pConfig->type != BattleConfig::SkillType::NEGATIVE)
            {
                if (pSkillData->canBreak || isDead(e.entity))
                {
                    breakSucceed = true;
                }
            }
        }
        
        if (breakSucceed)
        {
            CCLOG("skill cancelled of hero:%d name:%s", identify->id, skill->skill->getSkillData()->name.c_str());
         
            for (int soundId : skill->m_playingSoundPool)
            {
                AudioManager::getInstance()->stopEffect(soundId);
            }
            skill->m_playingSoundPool.clear();
            
            BattleComponent::Effect::Handle effect;
            for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(effect))
            {
                if (effect->fromId == identify->id)
                {
                    entity.destroy();
                }
            }
            
            BattleComponent::Attack::Handle attack;
            for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(attack))
            {
                if (attack->attackData.fromId == identify->id)
                {
                    entity.destroy();
                }
            }
            
            entity.destroy();
        }
    }
}

void SkillSystem::receive(const BattleEvent::TriggerSkillAction& e)
{
    if(_ENTITY_MANAGER.valid(e.id))
    {
        auto entity = _ENTITY_MANAGER.get(e.id);
        BattleComponent::Skill::Handle skill = entity.component<BattleComponent::Skill>();
        skill->skill->triggerAction(e.trigger);
    }
}

void SkillSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::Skill::Handle skill;

    for (entityx::Entity entity : es.entities_with_components(skill))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            if(skill->skill->shouldRemove())
            {
                entity.destroy();
            }
            else
            {
                skill->elapse += dt;
                skill->skill->update(es, skill->elapse, dt);
            }
        }
    }
}


```

### lua


``` lua

_REQUIRE("SanGuoLib")

local BattleScene = class("BattleScene", cc.load("mvc").ViewBase)

local model_game = _REQUIRE("model/model_game.lua")
local helper_battle_data = _REQUIRE("model.helper.helper_battle_data")

function BattleScene:onCreate()

    SanGuoAnimationLib.freeAllAdvancedAnimation();
    SanGuoAnimationLib.freeAllAdvancedAnimationAsset();

    local fight = helper_battle_data:prepareJSON()
    local autoQuickBattle = model_game:getAutoQuickBattle();
    local battleLayer = SanGuoLib.createBattle(fight, autoQuickBattle);
    self:addChild(battleLayer)
--    
--    if model_game:getAutoQuickBattle() then
--        battleLayer:setVisible(false)
--    end
--    Timer:runOnce(function()
--        if model_game:getAutoQuickBattle() then
--            self:showDlg("arena/RapidBattleWaiting")
--        end
--    end,0.01)
    
end

function BattleScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("MainScene")
        --        local test = my.MyClass:create()
        --        release_print("lua bind: " .. test:foo(99))
    end
end

function BattleScene:onExit()
    release_print("dddddd")
    --self:removeTexture()
end

return BattleScene


```


