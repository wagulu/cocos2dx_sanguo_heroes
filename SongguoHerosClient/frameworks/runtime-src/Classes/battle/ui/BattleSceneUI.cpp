//
//  BattleSceneUI.cpp
//  sanguoClient
//
//  Created by fuchenhao on 3/31/15.
//
//

#include "BattleSceneUI.h"
#include "base/CCScriptSupport.h"
#include "2d/CCSpriteFrame.h"
#include "2d/CCCamera.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "entityx.h"
#include "cocostudio/CocoStudio.h"
#include "DebugHelper.h"
#include "BattleWorld.h"
#include "AudioManager.h"
#include "CCLuaEngine.h"
#include "AudioSystem.h"
#include "AudioManager.h"
#include "BattleConfig.h"
#include "BattleCocosEvent.h"

#if (TEST_BATTLE)
#include "TestBattle.h"
#endif

USING_NS_CC;

BattleSceneUI::BattleSceneUI()
{
    Size frameSize = Director::getInstance()->getVisibleSize();
    Size dSize(1136, 640);
    Size size = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    
    const auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    if(battleFieldConfig.weather != "")
    {
        auto* particle = CSLoader::getInstance()->createNode("weather/" + battleFieldConfig.weather + ".csb");
        particle->setContentSize(frameSize);
        ui::Helper::doLayout(particle);
        particle->setName("Particle");
        particle->setVisible(false);
        this->addChild(particle);
        
        auto const childern = particle->getChildren();
        for(int i = 0; i < 100; i++)
        {
            for(auto child:childern)
            {
                child->update(0.040);
            }
        }
    }
    
    auto* panel = CSLoader::getInstance()->createNode("Scene/BattleScene.csb");
    panel->setContentSize(frameSize);
    ui::Helper::doLayout(panel);
    this->addChild(panel);
    
    m_battleSceneUITimeline = CSLoader::getInstance()->createTimeline("Scene/BattleScene.csb");
    panel->runAction(m_battleSceneUITimeline);
    
    m_battleSceneUI = panel->getChildByName<cocos2d::ui::Widget*>("Panel_BattleScene");
    m_battleSceneUI->setVisible(false);
    m_battleSceneUI->getChildByName<cocos2d::ui::Layout*>("Panel_B")->setTouchEnabled(false);
    m_battleSceneUI->getChildByName<cocos2d::ui::Layout*>("Panel_Bg")->setTouchEnabled(false);
    
    m_battleSceneUI->getChildByName("ProjectNode_Win")->setVisible(false);
    m_battleSceneUI->getChildByName("ProjectNode_Lose")->setVisible(false);
    
    initMapPanel();
    initPausePanel();
    initSkillButtons();
    initCountDown();
    initHeroInfoPanel();
    initControlPanel();
    initMultiHit();
    initSkillDescription();
    initSwallowTouchLayer();
    
    //layout
    Node* current;
    float scale = max((frameSize.height / dSize.height), (frameSize.width / dSize.width));
    
    //warn
    m_battleSceneUI->getChildByName("Panel_Bg")->getChildByName("Node_warn")->setVisible(false);
    
    //battle start
    m_battleStartAnimation = m_battleSceneUI->getChildByName("Node_Round");
    m_battleStartAnimationTimeline = CSLoader::getInstance()->createTimeline("Node/Node_Round.csb");
    m_battleStartAnimation->runAction(m_battleStartAnimationTimeline);
    m_battleStartAnimation->setVisible(false);
    
    //bg
    m_battleSceneUI->getChildByName("Panel_Bg")->setScale(scale);
    m_battlePausePanel->getChildByName("Panel_PauseMenu")->getChildByName("Panel_Bg")->setScale(scale);
    
    //map
    current = m_map;
    current->setPositionY(current->getPositionY() + (frameSize.height - dSize.height)/2);
    
    //top
    current = m_battleSceneUI->getChildByName("Panel_Top");
    current->setPositionY(current->getPositionY() + (frameSize.height - dSize.height)/2);
    
    current = m_battleSceneUI->getChildByName("Node_Animation_Skill2");
    current->setPositionY(current->getPositionY() + (frameSize.height - dSize.height)/2);
    
    current = m_battleSceneUI->getChildByName("Panel_Skill")->getChildByName("Panel_Bottom_9")->getChildByName("ProjectNode_1_5");
    current->setPositionY(current->getPositionY() + (frameSize.height - dSize.height));
    
    //bottom
    current = m_battleSceneUI->getChildByName("Panel_B");
    current->setPositionY(current->getPositionY() + (dSize.height - frameSize.height)/2);
    
    current = m_battleSceneUI->getChildByName("Panel_Bottom_Right");
    current->setPositionY(current->getPositionY() + (dSize.height - frameSize.height)/2);
    
    current = m_battleSceneUI->getChildByName("Panel_Bottom_Left");
    current->setPositionY(current->getPositionY() + (dSize.height - frameSize.height)/2);

    #if (TEST_BATTLE)
    //auto* btnTestPanel = TestBattle::getInstance()->createShowPanelButton();
    //this->addChild(btnTestPanel);
    #endif

    
    // hide strategy panel
    closeStrategyPanel(nullptr);
    
    // add listener
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_CLOSE_STRATEGY_PANEL, CC_CALLBACK_1(BattleSceneUI::closeStrategyPanel, this));
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_POPUPCOMBO, CC_CALLBACK_1(BattleSceneUI::onMultiHit, this));
    
    _ENTITY_EVENT.subscribe<BattleEvent::LockStatusChange>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::FireSkill>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::PlayHeroDeadEffect>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::PlayBattleResultEffect>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::ShowBattleSceneUI>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::HideBattleSceneUI>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::StartBattleComplete>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::UpdateBuffIcon>(*this);
}

BattleSceneUI::~BattleSceneUI()
{
    if (m_swallowTouchLayer)
    {
        m_swallowTouchLayer->release();
    }
    
    unschedule(schedule_selector(BattleSceneUI::displayBattleResult));
    unschedule(schedule_selector(BattleSceneUI::battleComplete));
    
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_CLOSE_STRATEGY_PANEL);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_POPUPCOMBO);
    
    _ENTITY_EVENT.unsubscribe<BattleEvent::LockStatusChange>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::FireSkill>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::PlayHeroDeadEffect>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::PlayBattleResultEffect>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::ShowBattleSceneUI>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::HideBattleSceneUI>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::StartBattleComplete>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::UpdateBuffIcon>(*this);
}

void BattleSceneUI::receive(const BattleEvent::ShowBattleSceneUI& e)
{
    showBattleSceneUI();
}

void BattleSceneUI::receive(const BattleEvent::HideBattleSceneUI& e)
{
    hideBattleSceneUI();
}

void BattleSceneUI::receive(const BattleEvent::StartBattleComplete& e)
{
    auto particle = this->getChildByName("Particle");
    if(particle) particle->setVisible(true);
}






//update buff icon
void BattleSceneUI::receive(const BattleEvent::UpdateBuffIcon& e)
{
    auto* entityL = _BATTLE_ENTITY.getEntity(LEFT_HERO);
    if(entityL)
    {
        if(entityL->has_component<BattleComponent::Buff>())
        {
            BattleComponent::Buff::Handle buff = entityL->component<BattleComponent::Buff>();
            int index = 0;
            for(BattleConfig::BuffData* buffData : buff->buffList)
            {
                if(buffData->buffIcon != "")
                {
                    index++;
                    auto* node = m_battleSceneUI->getChildByName("Panel_Bottom_Left")->getChildByName("Buff_L_" + Value(index).asString());
                    node->setVisible(true);
                    auto* buffIcon = node->getChildByName("Panel_RisingStar_Head")->getChildByName<ImageView*>("Image_Buff");
                    buffIcon->loadTexture("items/buff/" + buffData->buffIcon);
                    
                    if(index > 5) break;
                }
            }
            if(index < 5)
            {
                for(int i = index + 1; i <= 5; i++)
                {
                    auto* node = m_battleSceneUI->getChildByName("Panel_Bottom_Left")->getChildByName("Buff_L_" + Value(i).asString());
                    node->setVisible(false);
                }
            }
        }
        else
        {
            for(int i = 1; i <= 5; i++)
            {
                auto* node = m_battleSceneUI->getChildByName("Panel_Bottom_Left")->getChildByName("Buff_L_" + Value(i).asString());
                node->setVisible(false);
            }
        }
    }
    
    auto* entityR = _BATTLE_ENTITY.getEntity(RIGHT_HERO);
    if(entityR)
    {
        if(entityR->has_component<BattleComponent::Buff>())
        {
            BattleComponent::Buff::Handle buff = entityR->component<BattleComponent::Buff>();
            int index = 0;
            for(BattleConfig::BuffData* buffData : buff->buffList)
            {
                if(buffData->buffIcon != "")
                {
                    index++;
                    auto* node = m_battleSceneUI->getChildByName("Panel_Bottom_Right")->getChildByName("Buff_R_" + Value(index).asString());
                    node->setVisible(true);
                    auto* buffIcon = node->getChildByName("Panel_RisingStar_Head")->getChildByName<ImageView*>("Image_Buff");
                    buffIcon->loadTexture("items/buff/" + buffData->buffIcon);
                    
                    if(index > 5) break;
                }
            }
            if(index < 5)
            {
                for(int i = index + 1; i <= 5; i++)
                {
                    auto* node = m_battleSceneUI->getChildByName("Panel_Bottom_Right")->getChildByName("Buff_R_" + Value(i).asString());
                    node->setVisible(false);
                }
            }
        }
        else
        {
            for(int i = 1; i <= 5; i++)
            {
                auto* node = m_battleSceneUI->getChildByName("Panel_Bottom_Right")->getChildByName("Buff_R_" + Value(i).asString());
                node->setVisible(false);
            }
        }
    }
}







void BattleSceneUI::showBattleSceneUI()
{
    scheduleUpdate();
    
    m_isPlayingDownAnimation = false;
    m_isPlayingUpAnimation = true;
    m_isPlayingBattleStartAnimation = true;
    
    bindHeroInfoPanelWithHeros();
    bindSkillButtonsWithSkills();
    
    m_battleSceneUI->setVisible(true);
    m_battleSceneUITimeline->gotoFrameAndPlay(AnimationFrame::UP_START_FRAME, false);
    
    m_battleStartAnimation->setVisible(true);
    m_battleStartAnimationTimeline->gotoFrameAndPlay(0, false);
    
    m_map->setVisible(true);
    
    _ENTITY_EVENT.emit<BattleEvent::UpdateBuffIcon>();
    
    auto pauseContainer = m_battleSceneUI->getChildByName("Panel_Top");
    Button* stopFightButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Shift"));
    if(stopFightButton->isVisible())
    {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_START_DELEGATE);
    }

    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    if(battleFieldConfig.myTeam.soldier.soldierConfigData.soldierType != BattleConfig::SoldierType::ARCHER)
    {
        m_isHeroAttack = true;
        _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_ATTACK_SOLDIER, true);
        auto* panel = m_battleSceneUI->getChildByName<Layout*>("Panel_Right");
        auto* soldier = panel->getChildByName<ImageView*>("Sprite_Icon_soldiers_start");
        soldier->loadTexture("Assece/BattleScene/icon_soldiers_stop.png");
    }
}

void BattleSceneUI::checkBattleStartAnimationComplete()
{
    if (m_isPlayingBattleStartAnimation)
    {
        if (m_battleStartAnimationTimeline->getCurrentFrame() >= m_battleStartAnimationTimeline->getEndFrame())
        {
            m_battleStartAnimation->setVisible(false);
            
            m_isPlayingBattleStartAnimation = false;
        }
    }
}

void BattleSceneUI::checkShowBattleSceneUIComplete()
{
    if (m_isPlayingUpAnimation)
    {
        if (m_battleSceneUITimeline->getCurrentFrame() >= AnimationFrame::UP_END_FRAME)
        {
            m_battleSceneUITimeline->gotoFrameAndPause(AnimationFrame::UP_END_FRAME);
            
            if (m_swallowTouchLayer)
            {
                m_swallowTouchLayer->removeFromParent();
            }
            
            _ENTITY_EVENT.emit<BattleEvent::ShowBattleSceneUIComplete>();
        
            m_isPlayingUpAnimation = false;
        }
    }
}

void BattleSceneUI::hideBattleSceneUI()
{
    m_isPlayingUpAnimation = false;
    
    m_battleSceneUI->addChild(m_swallowTouchLayer);
    
//    m_isPlayingDownAnimation = true;
//    m_battleSceneUITimeline->gotoFrameAndPlay(AnimationFrame::DOWN_START_FRAME, false);
}

void BattleSceneUI::checkHideBattleSceneUIComplete()
{
    if (m_isPlayingDownAnimation)
    {
        if (m_battleSceneUITimeline->getCurrentFrame() >= AnimationFrame::DOWN_END_FRAME)
        {
            m_battleSceneUITimeline->gotoFrameAndPause(AnimationFrame::DOWN_END_FRAME);
            
            m_isPlayingDownAnimation = false;
            
            unscheduleUpdate();
        }
    }
}


void BattleSceneUI::initSwallowTouchLayer()
{
    m_swallowTouchLayer = cocos2d::LayerColor::create(Color4B(0, 0, 0, 0));
    m_swallowTouchLayer->retain();
    auto callback = [](cocos2d::Touch*, cocos2d::Event*)
    {
        return true;
    };
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_swallowTouchLayer);
    //_eventDispatcher->addEventListenerWithFixedPriority(listener,-1);
}

void BattleSceneUI::initSkillDescription()
{
    auto* node = m_battleSceneUI->getChildByName("Node_Animation_Skill2");
    m_skillDescAction = CSLoader::getInstance()->createTimeline("Node/Node_Animation_Skill2.csb");
    node->stopAllActions();
    node->runAction(m_skillDescAction);
    m_skillDescAction->gotoFrameAndPause(0);
    node->setVisible(false);
}

void BattleSceneUI::updateSkillDescription()
{
    if(m_skillDescAction->getCurrentFrame() >= 104)
    {
        m_skillDescAction->gotoFrameAndPause(0);
        auto* node = m_battleSceneUI->getChildByName("Node_Animation_Skill2");
        node->setVisible(false);

        switchMapPanel(false);
        
        CCLOG("skill effect end");
    }
}

void BattleSceneUI::receive(const BattleEvent::FireSkill& e)
{
    if (e.fromId == LEFT_HERO)
    {
        CCLOG("fire skill");
        switchMapPanel(true);
        
        auto* node = m_battleSceneUI->getChildByName("Node_Animation_Skill2");
        node->setVisible(true);
        ImageView* skillName = node->getChildByName("Panel_Skill_2")->getChildByName<cocos2d::ui::ImageView*>("Sprite_Skill_sglw");
        skillName->loadTexture("png/skillNames/" + e.pSkillData->pConfig->effect->asset + ".png");
    
        m_skillDescAction->gotoFrameAndPlay(0, false);
    }
}

void BattleSceneUI::receive(const BattleEvent::PlayHeroDeadEffect& e)
{
    //play effect
    Size frameSize = Director::getInstance()->getVisibleSize();
    float x = frameSize.width / 2;
    float y = frameSize.height / 2;
    
    auto pDefine = cocos2d::AssetDefineCache::getInstance()->getAsset("battleEffect");
    
    auto pAni0 = cocos2d::AdvancedAnimation::create(pDefine, "animation_full_kill_layer1");
    pAni0->setPositionX(x);
    pAni0->setPositionY(y);
    pAni0->setBlendMode(2);
    pAni0->playOnce(true);
    pAni0->scheduleUpdate();
    
    auto pAni1 = cocos2d::AdvancedAnimation::create(pDefine, "animation_full_kill_layer2");
    pAni1->setPositionX(x);
    pAni1->setPositionY(y);
    pAni1->setBlendMode(0);
    pAni1->playOnce(true);
    pAni1->scheduleUpdate();
    
    auto pAni2 = cocos2d::AdvancedAnimation::create(pDefine, "animation_full_kill_layer3");
    pAni2->setPositionX(x);
    pAni2->setPositionY(y);
    pAni2->setBlendMode(2);
    pAni2->playOnce(true);
    pAni2->scheduleUpdate();

    this->addChild(pAni0, 0);
    this->addChild(pAni1, 1);
    this->addChild(pAni2, 2);
}


void BattleSceneUI::initPausePanel()
{
    Size frameSize = Director::getInstance()->getVisibleSize();

    m_battlePausePanel = CSLoader::getInstance()->createNode("Scene/PauseMenuPanel.csb");
    m_battlePausePanel->setContentSize(frameSize);
    ui::Helper::doLayout(m_battlePausePanel);
    m_battlePausePanel->setVisible(false);
    this->addChild(m_battlePausePanel);
    
    auto panel = m_battlePausePanel->getChildByName("Panel_PauseMenu")->getChildByName("Panel_C")->getChildByName("Panel_Center");
    
    Button* continueButton = panel->getChildByName<Button*>("Button_Continue");//continue game
    continueButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::resumeBattle, this));
    
    Button* exitButton = panel->getChildByName<Button*>("Button_Menu");//exit battle
    exitButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::exitBattle, this));
    
    Button* enableEffectButton = panel->getChildByName<Button*>("Button_Voice");//enable effect
    enableEffectButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::enableEffect, this));
    
    Button* disableEffectButton = panel->getChildByName<Button*>("Button_Mute");//disable effect
    disableEffectButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::disableEffect, this));
    
    Button* enableMusicButton = panel->getChildByName<Button*>("Button_Music_1");//enable music
    enableMusicButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::enableMusic, this));
    
    Button* disableMusicButton = panel->getChildByName<Button*>("Button_Music_2");//disable music
    disableMusicButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::disableMusic, this));
    
    updateSoundButtonStatus();
    
    auto pauseContainer = m_battleSceneUI->getChildByName("Panel_Top");
    Button* pauseButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Stop"));//open pause panel
    pauseButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::showPausePanel, this));
    
    Button* autoFightButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Auto"));
    autoFightButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::startDelegate, this));
    
    Button* stopFightButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Shift"));
    stopFightButton->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::stopDelegate, this));
    
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    stopFightButton->setVisible(false);
    autoFightButton->setVisible(!battleFieldConfig.forceAutoFight);
    
    m_battlePausePanel->getChildByName("Panel_PauseMenu")->getChildByName("Panel_Bg")->getChildByName<ImageView*>("Image_BlackBg")->setTouchEnabled(true);
}


void BattleSceneUI::initHeroInfoPanel()
{
    auto leftHeroPanel = m_battleSceneUI->getChildByName<Widget*>("Panel_Bottom_Left");
    auto rightHeroPanel = m_battleSceneUI->getChildByName<Widget*>("Panel_Bottom_Right");
    
    leftHeroPanel->getChildByName<Widget*>("Panel_Role_InfoGroup")->setTouchEnabled(false);
    rightHeroPanel->getChildByName<Widget*>("Panel_Role_InfoGroup")->setTouchEnabled(false);

    ImageView* leftRole = leftHeroPanel->getChildByName<ImageView*>("Image_Role");
    ImageView* rightRole = rightHeroPanel->getChildByName<ImageView*>("Image_Role");
    leftRole->setTouchEnabled(true);
    leftRole->setEnabled(true);
    rightRole->setTouchEnabled(true);
    rightRole->setEnabled(true);
    leftRole->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::onHeroClick, this));
    rightRole->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::onHeroClick, this));
}

void BattleSceneUI::bindHeroInfoPanelWithHeros()
{
    auto leftHeroPanel = m_battleSceneUI->getChildByName<Widget*>("Panel_Bottom_Left");
    auto rightHeroPanel = m_battleSceneUI->getChildByName<Widget*>("Panel_Bottom_Right");
    
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    Node* current;
    std::vector<BattleConfig::HeroConfig*> heroConfigs;
    
    auto* pLeftEntity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
    if (pLeftEntity)
    {
        auto hero = pLeftEntity->component<BattleComponent::General>();
        leftHeroPanel->getChildByName<ImageView*>("Image_Role")->loadTexture("items/Monster_half/Monster_half_" + hero->config.asset + ".png");
        
        for (int i = 0; i < 5; i++)
        {
            current = leftHeroPanel->getChildByName("Panel_Role_InfoGroup")->getChildByName("Image_StarBg_" + Value(i + 1).asString());
            current->setVisible(i < hero->config.maxStar);
            current = leftHeroPanel->getChildByName("Panel_Role_InfoGroup")->getChildByName("Sprite_LevelStar" + Value(i + 1).asString());
            current->setVisible(i < hero->config.star);
        }
        
        auto& leftHeros = battleFieldConfig.myTeam.heros;
        heroConfigs.clear();
        for (int i = 0; i < leftHeros.size(); i++)
        {
            if (hero->config.id != leftHeros[i]->heroConfigData.id)
            {
                heroConfigs.push_back(leftHeros[i]);
            }
        }
        for (int i = 0; i < 2; i++)
        {
            current = leftHeroPanel->getChildByName("Node_Head" + Value(i+1).asString());
            if (heroConfigs.size() > i)
            {
                current->setVisible(true);
                current->getChildByName("Panel_RisingStar_Head")->getChildByName<ImageView*>("Image_Hero")->loadTexture("items/Monster/Monster_" + heroConfigs[i]->heroConfigData.asset + ".png");
                current->getChildByName("Panel_RisingStar_Head")->getChildByName("Image_Zhanbai")->setVisible(heroConfigs[i]->heroConfigData.currentHP <= 0);
            }
            else
            {
                current->setVisible(false);
            }
        }
        
        leftHeroPanel->getChildByName("Panel_Role_InfoGroup")->getChildByName<Sprite*>("Sprite_Job2")->setTexture("items/army/" + battleFieldConfig.leftSoldierIcon);
    }
    
    auto* pRightEntity = _BATTLE_ENTITY.getEntity(RIGHT_HERO);
    if (pRightEntity)
    {
        auto hero = pRightEntity->component<BattleComponent::General>();
        rightHeroPanel->getChildByName<ImageView*>("Image_Role")->loadTexture("items/Monster_half/Monster_half_" + hero->config.asset + ".png");
    
        for (int i = 0; i < 5; i++)
        {
            current = rightHeroPanel->getChildByName("Panel_Role_InfoGroup")->getChildByName("Image_StarBg_" + Value(i + 1).asString());
            current->setVisible(i < hero->config.maxStar);
            current = rightHeroPanel->getChildByName("Panel_Role_InfoGroup")->getChildByName("Sprite_LevelStar" + Value(i + 1).asString());
            current->setVisible(i < hero->config.star);
        }
        
        auto& rightHeros = battleFieldConfig.enemyTeam.heros;
        heroConfigs.clear();
        for (int i = 0; i < rightHeros.size(); i++)
        {
            if (hero->config.id != rightHeros[i]->heroConfigData.id)
            {
                heroConfigs.push_back(rightHeros[i]);
            }
        }
        for (int i = 0; i < 2; i++)
        {
            current = rightHeroPanel->getChildByName("Node_Head" + Value(i+1).asString());
            if (heroConfigs.size() > i)
            {
                current->setVisible(true);
                current->getChildByName("Panel_RisingStar_Head")->getChildByName<ImageView*>("Image_Hero")->loadTexture("items/Monster/Monster_" + heroConfigs[i]->heroConfigData.asset + ".png");
                current->getChildByName("Panel_RisingStar_Head")->getChildByName("Image_Zhanbai")->setVisible(heroConfigs[i]->heroConfigData.currentHP <= 0);
            }
            else
            {
                current->setVisible(false);
            }
        }
        
        rightHeroPanel->getChildByName("Panel_Role_InfoGroup")->getChildByName<Sprite*>("Sprite_Job2")->setTexture("items/army/" + battleFieldConfig.rightSoldierIcon);
    }
    
    ImageView* leftRole = leftHeroPanel->getChildByName<ImageView*>("Image_Role");
    ImageView* rightRole = rightHeroPanel->getChildByName<ImageView*>("Image_Role");
    leftRole->setScale9Enabled(false);
    rightRole->setScale9Enabled(false);
    
    leftRole->setContentSize(leftRole->getVirtualRendererSize());
    rightRole->setContentSize(rightRole->getVirtualRendererSize());
}

void BattleSceneUI::onHeroClick(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, true, 0.7, 1.05))
    {
        auto img = static_cast<Widget*>(pSender);
        
        //tag 338://敌方武将
        //tag 50://我方武将
        auto hero = img->getTag() == 338 ? RIGHT_HERO : LEFT_HERO;
        moveCameraToRole(hero, true);
    }
}

void BattleSceneUI::updateHeroInfo(int roleId)
{
    auto* pEntity = _BATTLE_ENTITY.getEntityEvenIfDead(roleId);
    if (pEntity == nullptr)
    {
        updateHpBar(roleId);
        return;
    }
    
    auto property = pEntity->component<BattleComponent::Property>();
    float hp = property->get(BattleConfig::Property::HP);
    float maxHp = property->getOriginal(BattleConfig::Property::HP);
    float mp = property->get(BattleConfig::Property::MP);
    float maxMp = property->getOriginal(BattleConfig::Property::MP);
    
    Widget* panel = static_cast<Widget*>(m_battleSceneUI->getChildByName(roleId == LEFT_HERO ? "Panel_Bottom_Left" : "Panel_Bottom_Right"));
    
    auto barHp = static_cast<LoadingBar*>(panel->getChildByName("LoadingBar_1"));
    auto barMp = static_cast<LoadingBar*>(panel->getChildByName("LoadingBar_2"));
    auto hpLabel = static_cast<Text*>(barHp->getChildByName(roleId == LEFT_HERO ? "Text_1" : "Text_7"));
    auto mPLabel = static_cast<Text*>(barMp->getChildByName(roleId == LEFT_HERO ? "Text_2" : "Text_8"));
    barHp->setPercent(hp / maxHp * 100.0f);
    barMp->setPercent(mp / maxMp * 100.0f);
    
    HeroInfo& info = (roleId == LEFT_HERO ? m_leftHeroInfo : m_rightHeroInfo);
    info.maxHp = maxHp;
    info.maxMp = maxMp;
    
    if(info.currentHp != hp)
    {
        hpLabel->setString(StringUtils::format("%d/%d", int(hp), int(maxHp)));
        info.currentHp = hp;
    }
    if(info.currentMp != mp)
    {
        mPLabel->setString(StringUtils::format("%d/%d", int(mp), int(maxMp)));
        info.currentMp = mp;
    }
    
    //血减少到一定百分比，显示特效（左边）
    //TODO, lrui, refactor, move to system, ugly and buggy code
    if(roleId == LEFT_HERO && m_hpLessThan20Percent == false && (hp / maxHp) <= 0.2f)
    {
        m_hpLessThan20Percent = true;
        auto* node = m_battleSceneUI->getChildByName("Panel_Bg")->getChildByName("Node_warn");
        auto* action = CSLoader::getInstance()->createTimeline("Node/Node_Animation_warn.csb");
        node->runAction(action);
        action->gotoFrameAndPlay(0, true);
    }
    
    //小兵数量显示
    int soilderNum = roleId == LEFT_HERO ? _BATTLE_ENTITY.getLeftSoldiers().size() : _BATTLE_ENTITY.getRightSoldiers().size();
    auto labelValue = static_cast<Label*>(panel->getChildByName("labelValue"));
    if (!labelValue)
    {
        Label* label = static_cast<Label*>(panel->getChildByName(roleId == LEFT_HERO ? "Text_9" : "Text_9_0"));
        label->setVisible(false);
        labelValue = cocos2d::Label::createWithBMFont("Time_Num.fnt", "2");
        panel->addChild(labelValue);
        labelValue->setName("labelValue");
        labelValue->setPosition(label->getPosition());
        labelValue->setAnchorPoint(Vec2(roleId == LEFT_HERO ? 0 : 1.0, 0.5));
        labelValue->setScaleX(roleId == LEFT_HERO ? 1 : -1);
    }
    labelValue->setString(Value(soilderNum).asString());
    
    //减血减MP效果
    int index = 100;
    auto barHpLayer = static_cast<LoadingBar*>(panel->getChildByName(roleId == LEFT_HERO ? "LoadingBar_1_1" : "LoadingBar_1_2"));
    auto barMpLayer = static_cast<LoadingBar*>(panel->getChildByName("LoadingBar_2_1"));
    
    if(hp <= 0)
    {
        updateHpBar(roleId);
        return;
    }
    
    if(hp == maxHp)
    {
        barHpLayer->setPercent(100);
        info.prevHp = index + 5;
    }
    else
    {
        if(info.prevHp <= index && info.prevHp >= hp / maxHp * index)
        {
            float stepHpNum = info.prevHp - hp / maxHp * index < 0.1 ? hp / maxHp * index : info.prevHp;
            barHpLayer->setPercent(stepHpNum);
            info.prevHp -= 0.1;
        }
        if(info.prevHp > index) info.prevHp -= 0.5;
    }
    
    if(mp == maxMp)
    {
        barMpLayer->setPercent(100);
        info.prevMp = index + 5;
    }
    else
    {
        if(info.prevMp <= index && info.prevMp >= mp / maxMp * index)
        {
            float stepMpNum = info.prevMp - mp / maxMp * index < 0.1 ? mp / maxMp * index : info.prevMp;
            barMpLayer->setPercent(stepMpNum);
            info.prevMp -= 0.1;
        }
        if(info.prevMp > index) info.prevMp -= 0.5;
    }
}

void BattleSceneUI::updateHpBar(int roleId)
{
    HeroInfo& info = roleId == LEFT_HERO ? m_leftHeroInfo : m_rightHeroInfo;

    //减血减MP效果
    int index = 100;
    Widget* panel = static_cast<Widget*>(m_battleSceneUI->getChildByName(roleId == LEFT_HERO ? "Panel_Bottom_Left" : "Panel_Bottom_Right"));
    auto barHpLayer = static_cast<LoadingBar*>(panel->getChildByName(roleId == LEFT_HERO ?"LoadingBar_1_1":"LoadingBar_1_2"));
    auto barMpLayer = static_cast<LoadingBar*>(panel->getChildByName("LoadingBar_2_1"));
    
    if(info.currentHp == info.maxHp)
    {
        barHpLayer->setPercent(100);
        info.prevHp = index + 5;
    }
    else
    {
        if(info.prevHp <= index && info.prevHp >= info.currentHp / info.maxHp * index)
        {
            float stepHpNum = info.prevHp - info.currentHp / info.maxHp * index < 0.4 ? info.currentHp / info.maxHp * index : info.prevHp;
            barHpLayer->setPercent(stepHpNum);
            info.prevHp -= 0.4;
        }
        if(info.prevHp > index) info.prevHp -= 0.5;
    }
    
    if(info.currentMp == info.maxMp)
    {
        barMpLayer->setPercent(100);
        info.prevMp = index + 5;
    }
    else
    {
        if(info.prevMp <= index && info.prevMp >= info.currentMp / info.maxMp * index)
        {
            float stepMpNum = info.prevMp - info.currentMp / info.maxMp * index < 0.4 ? info.currentMp / info.maxMp * index : info.prevMp;
            barMpLayer->setPercent(stepMpNum);
            info.prevMp -= 0.4;
        }
        if(info.prevMp > index) info.prevMp -= 0.5;
    }
}





void BattleSceneUI::initCountDown()
{
    auto panel = m_battleSceneUI->getChildByName("Panel_Skill")->getChildByName("Panel_Bottom_9");
    panel->getChildByName("FntLabel_TimeNum_4")->setVisible(false);
}

void BattleSceneUI::updateCountDown()
{    
    auto panel = m_battleSceneUI->getChildByName("Panel_Skill")->getChildByName("Panel_Bottom_9");
    auto labelValue = panel->getChildByName<Text*>("labelValue");
    if (!labelValue)
    {
        Text* label = panel->getChildByName<Text*>("FntLabel_TimeNum_4");
        
        labelValue = cocos2d::ui::Text::create("99", "Share/text/Marker Felt.ttf", 40);
        panel->addChild(labelValue);
        labelValue->setName("labelValue");
        labelValue->setPosition(label->getPosition());
        labelValue->enableOutline(Color4B(0, 0, 0, 0xFF), 4);
    }
    
    auto battleEntity = _BATTLE_ENTITY.getBattle();
    auto battle = battleEntity.component<BattleComponent::Battle>();
    int leftTime = floor(battle->totalTime - _BATTLE_STAGE.getBattleTime());
    if(leftTime < 0) leftTime = 0;
    labelValue->setString(Value(leftTime).asString());
}

void BattleSceneUI::receive(const BattleEvent::PlayBattleResultEffect& e)
{
    scheduleOnce(schedule_selector(BattleSceneUI::displayBattleResult), BATTLE_END_DELAY_TIME);
    scheduleOnce(schedule_selector(BattleSceneUI::battleComplete), RETURN_TO_LUA_DELAY);
}

void BattleSceneUI::displayBattleResult(float dt)
{
    auto isWin = BattleWorld::getInstance()->getBattleResult().isWin();
    auto* timeLine = CSLoader::getInstance()->createTimeline(isWin ? "Node/Node_Win.csb" : "Node/Node_Lose.csb");
    auto* node = m_battleSceneUI->getChildByName(isWin ? "ProjectNode_Win" : "ProjectNode_Lose");
    node->runAction(timeLine);
    node->setVisible(true);
    timeLine->gotoFrameAndPlay(0, false);
}

void BattleSceneUI::battleComplete(float dt)
{
    _ENTITY_EVENT.emit<BattleEvent::PlayBattleResultEffectComplete>();
}

void BattleSceneUI::exitBattle(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, true, 1.25))
    {
        _ENTITY_EVENT.emit<BattleEvent::ExitBattleBeforeEnd>();
        
        hidePausePanel();
    }
}

void BattleSceneUI::resumeBattle(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, true, 1.25))
    {
        hidePausePanel();
        _BATTLE_STAGE.resume();
        
        AudioManager::getInstance()->resumeAllEffect();
    }
}

void BattleSceneUI::hidePausePanel()
{
    m_battlePausePanelTimeline->gotoFrameAndPlay(65, false);
    m_pausePanelStatus = CLOSING;
}

void BattleSceneUI::showPausePanel(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type))
    {
        this->m_battlePausePanel->setVisible(true);
        m_battlePausePanelTimeline = CSLoader::getInstance()->createTimeline("Scene/PauseMenuPanel.csb");
        m_battlePausePanel->runAction(m_battlePausePanelTimeline);
        m_battlePausePanelTimeline->gotoFrameAndPlay(0, false);
        m_pausePanelStatus = OPENING;

        _BATTLE_STAGE.pause();
        
        AudioManager::getInstance()->pauseAllEffect();
    }
}

void BattleSceneUI::updatePausePanel()
{
    switch(m_pausePanelStatus)
    {
        case OPENING:
            if(m_battlePausePanelTimeline->getCurrentFrame() >= 20)
            {
                m_battlePausePanelTimeline->gotoFrameAndPause(20);
                m_pausePanelStatus = OPEN;
            }
            break;
        case OPEN:
            break;
        case CLOSING:
            if(m_battlePausePanelTimeline->getCurrentFrame() >= 79)
            {
                this->m_battlePausePanel->setVisible(false);
                m_pausePanelStatus = CLOSE;
            }
            break;
        case CLOSE:
            break;
    }
}

void BattleSceneUI::startDelegate(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type))
    {
        auto pauseContainer = m_battleSceneUI->getChildByName("Panel_Top");
        Button* autoFightButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Auto"));
        Button* stopFightButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Shift"));
        
        stopFightButton->setVisible(true);
        autoFightButton->setVisible(false);
        
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_START_DELEGATE);
    }
}

void BattleSceneUI::stopDelegate(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type))
    {
        auto pauseContainer = m_battleSceneUI->getChildByName("Panel_Top");
        Button* autoFightButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Auto"));
        Button* stopFightButton = static_cast<Button*>(pauseContainer->getChildByName("Button_Shift"));
        
        stopFightButton->setVisible(false);
        autoFightButton->setVisible(true);
        
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_STOP_DELEGATE);
    }
}



void BattleSceneUI::enableEffect(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, true, 1.25))
    {
        AudioManager::getInstance()->enableEffect();
        updateSoundButtonStatus();
    }
}

void BattleSceneUI::disableEffect(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, true, 1.25))
    {
        AudioManager::getInstance()->disableEffect();
        updateSoundButtonStatus();
    }
}

void BattleSceneUI::enableMusic(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, true, 1.25))
    {
        AudioManager::getInstance()->enableMusic();
        updateSoundButtonStatus();
    }
}

void BattleSceneUI::disableMusic(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, true, 1.25))
    {
        AudioManager::getInstance()->disableMusic();
        updateSoundButtonStatus();
    }
}

void BattleSceneUI::updateSoundButtonStatus()
{
    auto panel = m_battlePausePanel->getChildByName("Panel_PauseMenu")->getChildByName("Panel_C")->getChildByName("Panel_Center");
    
    Button* enableEffectButton = panel->getChildByName<Button*>("Button_Voice");//enable effect
    enableEffectButton->setVisible(!AudioManager::getInstance()->isEffectEnable());
    
    Button* disableEffectButton = panel->getChildByName<Button*>("Button_Mute");//disable effect
    disableEffectButton->setVisible(AudioManager::getInstance()->isEffectEnable());
    
    Button* enableMusicButton = panel->getChildByName<Button*>("Button_Music_1");//enable music
    enableMusicButton->setVisible(!AudioManager::getInstance()->isMusicEnable());
    
    Button* disableMusicButton = panel->getChildByName<Button*>("Button_Music_2");//disable music
    disableMusicButton->setVisible(AudioManager::getInstance()->isMusicEnable());

}






void BattleSceneUI::initControlPanel()
{
    auto* panel = m_battleSceneUI->getChildByName<Layout*>("Panel_Right");
    auto* hero = panel->getChildByName<ImageView*>("Sprite_Icon_wujiang_start");
    auto* soldier = panel->getChildByName<ImageView*>("Sprite_Icon_soldiers_start");
    soldier->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::switchSoldierAttack, this));
    hero->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::switchHeroAttack, this));
    
    auto* sprite = static_cast<Sprite*>(m_battleSceneUI->getChildByName("Sprite_AddIcon_Bg"));
    for (int i = 1; i < 5; i++)
    {
        Widget* icon = static_cast<Widget*>(sprite->getChildByName("btn_op_"+Value(i).asString()));
        if(icon)icon->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::switchSoldierStrategy, this));
    }
    
    auto* switchIcon = static_cast<ImageView*>(m_battleSceneUI->getChildByName("img_add"));
    m_strategyStartX = sprite->getPosition().x;
    
    //sprite->setVisible(false);
    Size size = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    sprite->setPositionX(size.width + 100);
    switchIcon->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::switchStrategyPanel, this));
    switchIcon->setVisible(true);
    
    if (BattleConfig::getInstance()->getBattleFieldConfig().battleType == BattleConfig::BattleType::DUEL)
    {
        soldier->setVisible(false);
    }
}

//侦听点击任何界面，自动收起来
void BattleSceneUI::closeStrategyPanel(EventCustom* event)
{
    m_showStrategyPanel = false;
    
    auto* icon = static_cast<ImageView*>(m_battleSceneUI->getChildByName("img_add"));
    
    icon->loadTexture("Assece/BattleScene/Btn_add.png");
    
    auto* sprite = static_cast<Sprite*>(m_battleSceneUI->getChildByName("Sprite_AddIcon_Bg"));
    sprite->setPositionX(Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width + 100);
}

void BattleSceneUI::switchStrategyPanel(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type, false))
    {
        m_showStrategyPanel = !m_showStrategyPanel;
        
        auto* icon = static_cast<ImageView*>(m_battleSceneUI->getChildByName("img_add"));
        icon->loadTexture(m_showStrategyPanel ? "Assece/BattleScene/Btn_minus.png" : "Assece/BattleScene/Btn_add.png");
        
        auto* sprite = static_cast<Sprite*>(m_battleSceneUI->getChildByName("Sprite_AddIcon_Bg"));
        sprite->setPositionX(m_showStrategyPanel ? m_strategyStartX : Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width + 100);
    }
}

void BattleSceneUI::switchHeroAttack(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type))
    {
        auto img = static_cast<Widget*>(pSender);
        m_isSoldierAttack = !m_isSoldierAttack;
        
        _CAMERA_MANAGER.initSmartFollowSolider();

        if (m_isSoldierAttack)
        {
            std::string eventName = AUDIOEVENTS_ARMY_ASSAULT;
            std::string triggerName = ""; //getHeroSoldierAudioTriggerName(entity);
            BattleEvent::BattleSound soundEvent(eventName, triggerName, entityx::Entity());
            _ENTITY_EVENT.emit(soundEvent);
            
            moveCameraToRole(LEFT_HERO, true);
            
            static_cast<ImageView*>(img)->loadTexture("Assece/BattleScene/icon_wujiang_stop.png");
             _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_RUSH_OUT, LEFT_HERO);
        }
        else
        {
            static_cast<ImageView*>(img)->loadTexture("Assece/BattleScene/icon_wujiang_start.png");
            _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_STOP, LEFT_HERO);
        }
    }
}

void BattleSceneUI::switchSoldierAttack(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type))
    {
        auto img = static_cast<Widget*>(pSender);
        m_isHeroAttack = !m_isHeroAttack;
        
        if (m_isHeroAttack)
        {
            std::string eventName = AUDIOEVENTS_HERO_ASSAULT;
            std::string triggerName = "hero"; //getHeroSoldierAudioTriggerName(entity);
            BattleEvent::BattleSound soundEvent(eventName, triggerName, entityx::Entity());
            _ENTITY_EVENT.emit(soundEvent);
            
            
            static_cast<ImageView*>(img)->loadTexture("Assece/BattleScene/icon_soldiers_stop.png");
            _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_ATTACK_SOLDIER, true);
        }
        else
        {
            static_cast<ImageView*>(img)->loadTexture("Assece/BattleScene/icon_soldiers_start.png");
            _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_STOP, true);
        }
    }
}

void BattleSceneUI::switchSoldierStrategy(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type))
    {
        auto img = static_cast<Widget*>(pSender);
        
        closeStrategyPanel(nullptr);
        if(img->getTag() != 30002) _CAMERA_MANAGER.initSmartFollowSolider();
        
        switch (img->getTag())
        {
            case 30000://护主
            {
                _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_PROTECT_HERO, true);
                break;
            }
            case 30001://击将
            {
                _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_ATTACK_HERO, true);
                break;
            }
            case 30002://集中
            {
                _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_RECENTER, true);
                break;
            }
            case 30003://后退
            {
                _STRATEGY_CONTROL.setSoldierControl(StrategyControl::SOLDIER_BACK, true);
                break;
            }
            default:
                break;
        }
    }
}





void BattleSceneUI::initMultiHit()
{
    auto panel = m_battleSceneUI->getChildByName("Panel_Skill")->getChildByName("Panel_Bottom_9");
    auto node = panel->getChildByName("ProjectNode_1_5");
    node->setVisible(false);
    
    m_currentAnimationStep = 0;
}

void BattleSceneUI::onMultiHit(EventCustom* event)
{
    auto userData = event->getUserData();
    int i = *(int*)userData;
    
    setCurrentHit(i);
}

void BattleSceneUI::setCurrentHit(int num)
{
    auto panel = m_battleSceneUI->getChildByName("Panel_Skill")->getChildByName("Panel_Bottom_9");
    auto node = panel->getChildByName("ProjectNode_1_5");
    node->setVisible(true);
    
    Widget* textPanel = static_cast<Widget*>(node->getChildByName("Panel_text_continue"));
    auto labelValue = static_cast<Label*>(textPanel->getChildByName("labelValue"));
    if (!labelValue)
    {
        Label* textNum = static_cast<Label*>(textPanel->getChildByName("FntLabel_Continue"));
        textNum->setVisible(false);
        
        labelValue = cocos2d::Label::createWithBMFont("Continue.fnt", "2");
        textPanel->addChild(labelValue);
        labelValue->setName("labelValue");
        labelValue->setPosition(textNum->getPosition());
    }
    labelValue->setString(Value(num).asString());
    
    m_currentAnimationStep++;
    if(m_currentAnimationStep == 4) m_currentAnimationStep = 0;
    
    m_multiHitAction = CSLoader::getInstance()->createTimeline("Node/Node_Animation_Hit.csb");
    node->runAction(m_multiHitAction);
    m_multiHitAction->gotoFrameAndPlay(m_currentAnimationStep * 6, false);
    m_isMultiHitPlaying = true;
    m_showMultiHitElapse = 0;
}

void BattleSceneUI::updateMultiHit(float dt)
{
    if (!m_isMultiHitPlaying)
    {
        if(m_showMultiHitElapse < 1.2f && m_showMultiHitElapse + dt >= 1.2f)
        {
            auto panel = m_battleSceneUI->getChildByName("Panel_Skill")->getChildByName("Panel_Bottom_9");
            auto node = panel->getChildByName("ProjectNode_1_5");
            node->setVisible(false);
        }
        m_showMultiHitElapse += dt;
        return;
    }
    
    int targetFrame = (m_currentAnimationStep + 1) * 6;
    if(m_multiHitAction->getCurrentFrame() >= targetFrame)
    {

        m_multiHitAction->gotoFrameAndPause(targetFrame);
        m_isMultiHitPlaying = false;
    }
}

void BattleSceneUI::initMapPanel()
{
    m_map = cocos2d::Node::create();
    m_map->setVisible(false);
    m_map->setPositionY(536);
    m_map->setPositionX(400);
    m_battleSceneUI->addChild(m_map, 0);
    
    m_mapBg = cocos2d::ui::Scale9Sprite::create("png/bg/small_map_bg.png");
    m_mapBg->setAnchorPoint(cocos2d::Vec2(0, 0));
    m_mapBg->setContentSize(cocos2d::Size(400, 120));
    m_mapBg->setPosition(cocos2d::Vec2(-30, -20));
    m_mapBg->setOpacity(90);
    m_map->addChild(m_mapBg);
    
    auto frame = cocos2d::Sprite::create("png/frame.png");
    frame->setPositionY(-4.5);
    frame->setAnchorPoint(cocos2d::Vec2(0.5, 0));
    frame->setTag(MAP_FRAME);
    m_map->addChild(frame);
    
    m_mapStartPos = m_map->getPosition();
}

void BattleSceneUI::switchMapPanel(bool hide)
{
    if(m_isMapHide == hide) return;
    
    m_isMapHide = hide;
    MoveTo* targetPosition;
    
    if(hide)
    {
        targetPosition = MoveTo::create(0.5f, Vec2(m_mapStartPos.x, Director::getInstance()->getOpenGLView()->getDesignResolutionSize().height + 100));
    }
    else
    {
        targetPosition = MoveTo::create(0.5f, Vec2(m_mapStartPos.x, m_mapStartPos.y));
    }
    m_map->runAction(CCSequence::create(targetPosition,  NULL));
}








bool BattleSceneUI::checkTouchType(cocos2d::Ref *pSender,Widget::TouchEventType type, bool closeStrategyPanel, float orginScale, float targetScale)
{
    if(closeStrategyPanel) this->closeStrategyPanel(nullptr);
    
    auto img = static_cast<Widget*>(pSender);
    if(type == cocos2d::ui::Widget::TouchEventType::BEGAN)
    {
        img->stopAllActions();
        img->setScale(1.0f);
        img->setScale(targetScale * orginScale);
        img->runAction(ScaleTo::create(0.2, 1.0 * orginScale));
        return false;
    }
    else if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
    {
        return true;
    }
    return false;
}






void BattleSceneUI::moveCameraToRole(int id, bool follow)
{
    if (follow)
    {
        if (_CAMERA_MANAGER.isCameraUserControlLocked())
        {
            _ENTITY_EVENT.emit<BattleEvent::SetCameraModeForLockEvent>(false, id);
        }
        else
        {
            auto* pEntity = _BATTLE_ENTITY.getEntity(id);
            if (pEntity)
            {
                _ENTITY_EVENT.emit<BattleEvent::CameraFollowEntity>(*pEntity, true, !_CAMERA_MANAGER.isCameraUserControlLocked());
            }
        }
    }
    else
    {
        BattleComponent::Position::Handle position = _BATTLE_ENTITY.getEntity(id)->component<BattleComponent::Position>();
        //点击我方武将和敌方武将头像，镜头切换至战场武将身上(第一个参数是目标武将的x坐标，第四个参数表现切换过程中需要的时间秒数)
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(position->x, CameraManager::INIT_CAMERA_Z, CameraManager::INIT_CAMERA_Y,
                                                                  0.1f, 0,
                                                                  CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                  CameraManager::INIT_CAMERA_ROTATE, CameraManager::LINEAR, -1);
    }
}







void BattleSceneUI::update(float dt)
{
    checkBattleStartAnimationComplete();
    checkShowBattleSceneUIComplete();
    checkHideBattleSceneUIComplete();
    
    updateHeroInfo(LEFT_HERO);
    updateHeroInfo(RIGHT_HERO);
    updateSkillButtons();
    updateCountDown();
    updateMultiHit(dt);
    updateSkillDescription();
    updatePausePanel();
}






void BattleSceneUI::initSkillButtons()
{
    m_battleSceneUI->getChildByName<Layout*>("Panel_Skill")->setTouchEnabled(false);
    auto panelBottom = m_battleSceneUI->getChildByName("Panel_Skill")->getChildByName<Layout*>("Panel_Bottom_9");
    panelBottom->setTouchEnabled(false);
    
    for (int i = 0; i < 4; i++)
    {
        auto* skillButton = panelBottom->getChildByName("Node_Animation_Skill_" + Value(i+1).asString() + "_" + Value(i+1).asString());
        m_skillButtons.push_back(skillButton);
        m_skillButtonInitY = skillButton->getPositionY();
        
        auto* effectAction = CSLoader::getInstance()->createTimeline("Node/Node_Animation_Skill.csb");
        skillButton->runAction(effectAction);
        m_skillButtonAnimation.push_back(effectAction);
        
        auto iconItem = skillButton->getChildByName("Panel_1")->getChildByName<ImageView*>("Image_Icon");
        iconItem->setTag(i);
        iconItem->setTouchEnabled(true);
        iconItem->setEnabled(true);
        iconItem->addTouchEventListener(CC_CALLBACK_2(BattleSceneUI::onSkillButtonClick, this));
        m_skillButtonLoadingBar.push_back(iconItem->getChildByName<LoadingBar*>("LoadingBar_1"));
        m_skillButtonIconWidth = iconItem->getContentSize().width;
    }
}

void BattleSceneUI::bindSkillButtonsWithSkills()
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
    if (pEntity)
    {
        auto& skills = pEntity->component<BattleComponent::GasGather>()->skills;
        
        m_positiveSkills.clear();
        m_skillButtonAnimationStatus.clear();
        int totalSkills = skills.size();
        for (int i = 0; i < totalSkills; i++)
        {
            if (skills[i]->pConfig->type == BattleConfig::SkillType::POSITIVE)
            {
                m_positiveSkills.push_back(i);
                m_skillButtonAnimationStatus.push_back(ButtonAnimationStatus::ANIMATION_DISABLE);
            }
        }
        
        int skillNum = MIN(4, m_positiveSkills.size());
        Size size = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
        
        for (int i = 0; i < 4; i++)
        {
            auto* skillButton = m_skillButtons[i];
            
            if (i < skillNum)
            {
                auto x = (size.width - skillNum * m_skillButtonIconWidth) / 2 + m_skillButtonIconWidth / 2 + (SKILL_ICON_PADDING + m_skillButtonIconWidth) * i - 30;
                skillButton->setPositionX(x);
                
                int mpNum = skills[m_positiveSkills[i]]->mpCost;
                skillButton->getChildByName("Image_Num_Bg")->getChildByName<Text*>("Text_1")->setString(Value(mpNum).asString());
                
                
                cocos2d::ui::LoadingBar* skillIcon = skillButton->getChildByName("Panel_1")->getChildByName("Image_Icon")->getChildByName<LoadingBar*>("LoadingBar_Skill");
                auto skillIconPath = "items/icon_skill/icon_skill_" + Value(skills[m_positiveSkills[i]]->id).asString();
                skillIcon->loadTexture(skillIconPath + ".png");
                
                skillButton->setVisible(true);
            }
            else
            {
                skillButton->setVisible(false);
            }
        }
    }
}

void BattleSceneUI::onSkillButtonClick(cocos2d::Ref *pSender,Widget::TouchEventType type)
{
    if(checkTouchType(pSender, type))
    {
        auto img = static_cast<Widget*>(pSender);
        
        auto* pEntity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
        if (pEntity)
        {
            auto gasGather = pEntity->component<BattleComponent::GasGather>();
            auto* pSkillData = gasGather->skills[m_positiveSkills[img->getTag()]];
            _ENTITY_EVENT.emit<BattleEvent::PreFireSkill>(pSkillData->id, LEFT_HERO);
        }
    }
}

// 0 技能集气未完成
// 0-35 技能放光圈//
// 35-70 技能就绪状态//
void BattleSceneUI::updateSkillButtons()
{
    auto* pEntity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
    if (pEntity)
    {
        //LoadingBar_Skill//skill icon
        auto gasGather = pEntity->component<BattleComponent::GasGather>();
        int skillNum = MIN(4, m_positiveSkills.size());
        for(int i = 0; i < skillNum; i++)
        {
            auto* pSkillData = gasGather->skills[m_positiveSkills[i]];
            auto bar = this->m_skillButtonLoadingBar[i];
            bar->setPercent((pSkillData->energyCost - pSkillData->energy) / pSkillData->energyCost * 100);
            
            if(pSkillData->isActive)
            {
                this->m_skillButtons[i]->setColor(cocos2d::Color3B(255, 255, 255));
            }
            else
            {
                this->m_skillButtons[i]->setColor(cocos2d::Color3B(140, 140, 140));
            }
            
            if(pSkillData->energyCost <= pSkillData->energy)
            {
                if(this->m_skillButtonAnimationStatus[i] == ANIMATION_DISABLE)
                {
                    this->m_skillButtonAnimation[i]->gotoFrameAndPlay(0, false);
                    this->m_skillButtonAnimationStatus[i] = ANIMATION_READY;
                }
                else
                {
                    if(this->m_skillButtonAnimation[i]->getCurrentFrame() >= 70)
                    {
                        this->m_skillButtonAnimation[i]->gotoFrameAndPlay(35, false);
                    }
                }
            }
            else
            {
                if(this->m_skillButtonAnimationStatus[i] != ANIMATION_DISABLE)
                {
                    this->m_skillButtonAnimationStatus[i] = ANIMATION_DISABLE;
                    this->m_skillButtonAnimation[i]->gotoFrameAndPause(0);
                }
            }
        }
    }
}

void BattleSceneUI::receive(const BattleEvent::LockStatusChange& e)
{
    int skillNum = MIN(4, m_positiveSkills.size());
    for(int i = 0; i < skillNum; i++)
    {
        MoveTo* position;
        float startX = m_skillButtons[i]->getPositionX();;
        
        if(e.lock)
        {
            //move up
             position = MoveTo::create(SKILL_ICON_MOVE_TIME, Vec2(startX, m_skillButtonInitY - SKILL_ICON_MOVE_DISTANCE));
        }
        else
        {
            //move down
            position = MoveTo::create(SKILL_ICON_MOVE_TIME, Vec2(startX, m_skillButtonInitY));
        }
        m_skillButtons[i]->runAction(Sequence::create(position, NULL));
    }
}



