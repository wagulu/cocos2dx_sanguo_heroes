//
//  BattleDialogueUI.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/23/15.
//
//

#include "BattleDialogueUI.h"
#include "BattleWorld.h"
#include "ProjectConfig.h"

BattleDialogueUI::BattleDialogueUI()
{
    m_currentDialogue = 0;
    
    Size frameSize = Director::getInstance()->getVisibleSize();
    Size dSize(1136, 640);

    m_battleDialogueUI = CSLoader::getInstance()->createNode("Scene/DialoguePopup.csb");
    m_battleDialogueUI->setContentSize(frameSize);
    ui::Helper::doLayout(m_battleDialogueUI);
    this->addChild(m_battleDialogueUI);
    
    cocos2d::ui::ImageView* background = m_battleDialogueUI->getChildByName("DialoguePopup")->getChildByName("Panel_Bg")->getChildByName<cocos2d::ui::ImageView*>("Image_Bg");
    background->addClickEventListener(CC_CALLBACK_1(BattleDialogueUI::next, this));
    background->setTouchEnabled(true);
    
    m_battleDialogueUI->setVisible(false);
    
    //layout
    Node* panel = m_battleDialogueUI->getChildByName("DialoguePopup");
    Node* current;
    float scale = max((frameSize.height / dSize.height), (frameSize.width / dSize.width));
    
    //bg
    panel->getChildByName("Panel_Bg")->setScale(scale);
    
    //bottom
    current = panel->getChildByName("Panel_C");
    current->setPositionY(current->getPositionY() + (dSize.height - frameSize.height)/2);
    
    current = panel->getChildByName("Panel_Right");
    current->setPositionY(current->getPositionY() + (dSize.height - frameSize.height)/2);
    
    current = panel->getChildByName("Panel_Left");
    current->setPositionY(current->getPositionY() + (dSize.height - frameSize.height)/2);

    _ENTITY_EVENT.subscribe<BattleEvent::ShowStartDialogue>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::ShowWinDialogue>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::ShowLoseDialogue>(*this);
}

BattleDialogueUI::~BattleDialogueUI()
{
    _ENTITY_EVENT.unsubscribe<BattleEvent::ShowStartDialogue>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::ShowWinDialogue>(*this);
    _ENTITY_EVENT.unsubscribe<BattleEvent::ShowLoseDialogue>(*this);
}

void BattleDialogueUI::receive(const BattleEvent::ShowStartDialogue& e)
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    m_dialogues = &(battleFieldConfig.enter);
    if(SKIP_DIALOG || m_dialogues->size() == 0)
    {
        _ENTITY_EVENT.emit<BattleEvent::DialogueStartComplete>();
        return;
    }
    m_type = START;
    m_currentDialogue = 0;
    showCurrent();
}

void BattleDialogueUI::receive(const BattleEvent::ShowWinDialogue& e)
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    m_dialogues = &(battleFieldConfig.win);
    if(SKIP_DIALOG || m_dialogues->size() == 0)
    {
        _ENTITY_EVENT.emit<BattleEvent::DialogueResultComplete>();
        return;
    }
    m_type = WIN;
    m_currentDialogue = 0;
    showCurrent();
}

void BattleDialogueUI::receive(const BattleEvent::ShowLoseDialogue& e)
{
    auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    m_dialogues = &(battleFieldConfig.lose);
    if(SKIP_DIALOG || m_dialogues->size() == 0)
    {
        _ENTITY_EVENT.emit<BattleEvent::DialogueResultComplete>();
        return;
    }
    m_type = LOSE;
    m_currentDialogue = 0;
    showCurrent();
}

void BattleDialogueUI::showCurrent()
{
    m_battleDialogueUI->setVisible(true);
    
    //Panel_Left//Panel_L
    //Image_R//role
    //Text_1//name
    //Text_2//content
    //ProjectNode_1//next
    
    //Panel_Right/Panel_R
    //Image_R//role
    //Text_1//name
    //Text_2//content
    //ProjectNode_2//next
    
    m_dialogueTimeline = CSLoader::getInstance()->createTimeline("Scene/DialoguePopup.csb");
    m_battleDialogueUI->runAction(m_dialogueTimeline);
    auto* dialogue = m_dialogues->at(m_currentDialogue);
    int startFrame = dialogue->side == BattleConfig::SIDE_LEFT ? 0 : 170;
    m_dialogueTimeline->gotoFrameAndPlay(startFrame, false);
    
    m_crrentStep = APPEARING;
    this->scheduleUpdate();
    
    cocos2d::ui::Text* textName;
    cocos2d::ui::Text* textContent;
    cocos2d::ui::ImageView* role;
    cocos2d::Node* panel;
    
    if(dialogue->side == BattleConfig::SIDE_LEFT)
    {
        panel = m_battleDialogueUI->getChildByName("DialoguePopup")->getChildByName("Panel_Left")->getChildByName("Panel_L");
    }
    else
    {
        panel = m_battleDialogueUI->getChildByName("DialoguePopup")->getChildByName("Panel_Right")->getChildByName("Panel_R");
    }
    
    textName = panel->getChildByName<cocos2d::ui::Text*>("Text_1");
    textContent = panel->getChildByName<cocos2d::ui::Text*>("Text_2");
    role = panel->getChildByName<cocos2d::ui::ImageView*>("Image_R");
    role->loadTexture("items/Monster_half/Monster_half_" + dialogue->role + ".png");
    
    textName->setString(dialogue->name);
    textContent->setString(dialogue->content);
    
    if (m_type != START)
    {
        int id = dialogue->side == BattleConfig::SIDE_LEFT ? LEFT_HERO : RIGHT_HERO;
        if(_BATTLE_ENTITY.getEntity(id))
        {
            BattleComponent::Position::Handle position = _BATTLE_ENTITY.getEntity(id)->component<BattleComponent::Position>();
            _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(position->x , CameraManager::INIT_CAMERA_Z, CameraManager::INIT_CAMERA_Y,
                                                                      0.1f, 0,
                                                                      CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                                      CameraManager::INIT_CAMERA_ROTATE, CameraManager::LINEAR, -1);
        }
    }
}

void BattleDialogueUI::update(float dt)
{
    int currentFrame = m_dialogueTimeline->getCurrentFrame();
    auto* dialogue = m_dialogues->at(m_currentDialogue);
    int endFrame;
    
    //left
    //frame 0~80
    //dispear 160~170
    
    //right
    //frame 170~235
    //disappear 285~295
    
    switch(m_crrentStep)
    {
        case APPEARING:
            endFrame = dialogue->side == BattleConfig::SIDE_LEFT ? 80 : 235;
            if(currentFrame >= endFrame)
            {
                m_crrentStep = PAUSING;
                m_dialogueTimeline->gotoFrameAndPause(endFrame);
                m_canShowNext = true;
            }
            break;
            
        case PAUSING:
            endFrame = dialogue->side == BattleConfig::SIDE_LEFT ? 80 : 235;
            m_dialogueTimeline->gotoFrameAndPause(endFrame);
            break;
        
        case DISAPPEARING:
            endFrame = dialogue->side == BattleConfig::SIDE_LEFT ? 170 : 295;
            if(currentFrame >= endFrame - 1)
            {
                complete();
            }
            break;
    }
}

void BattleDialogueUI::complete()
{
    this->unscheduleUpdate();
    
    m_currentDialogue++;
    if(m_currentDialogue >= m_dialogues->size())
    {
        //all complete
        m_battleDialogueUI->setVisible(false);
        if(m_type == START)
        {
            _ENTITY_EVENT.emit<BattleEvent::DialogueStartComplete>();
        }
        else
        {
            _ENTITY_EVENT.emit<BattleEvent::DialogueResultComplete>();
        }
    }
    else
    {
        showCurrent();
    }
}

void BattleDialogueUI::next(cocos2d::Ref* ref)
{
    if(m_canShowNext)
    {
        auto* dialogue = m_dialogues->at(m_currentDialogue);
        int startFrame = dialogue->side == BattleConfig::SIDE_LEFT ? 160 : 285;
        m_dialogueTimeline->gotoFrameAndPlay(startFrame, false);
        m_crrentStep = DISAPPEARING;
        m_canShowNext = false;
    }
}

















