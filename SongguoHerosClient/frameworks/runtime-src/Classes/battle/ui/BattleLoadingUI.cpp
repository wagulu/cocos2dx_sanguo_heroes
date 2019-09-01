//
//  BattleLoadingUI.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/20/15.
//
//

#include "BattleLoadingUI.h"

BattleLoadingUI::BattleLoadingUI()
{
    Size frameSize = Director::getInstance()->getVisibleSize();
    Size dSize(1136, 640);
    
    auto* loading = CSLoader::getInstance()->createNode("Scene/LoginScene.csb");
    loading->setContentSize(frameSize);
    ui::Helper::doLayout(loading);
    this->addChild(loading);
    
    loading->setName("loginPanel");
    loading->getChildByName("panel_login")->getChildByName("Panel_Center")->getChildByName("Panel_Function")->removeFromParent();
    auto* bottom = loading->getChildByName("panel_login")->getChildByName("Panel_Bottom");
    bottom->getChildByName("Text_Tip")->removeFromParent();
    auto* loadingBar = bottom->getChildByName<cocos2d::ui::LoadingBar*>("LoadingBar");
    loadingBar->setPercent(10.0);
    auto* progressText = bottom->getChildByName<cocos2d::ui::Text*>("Text_Progress");
    progressText->setString("10 %");
    auto* bg = loading->getChildByName("panel_login")->getChildByName("Panel_Bg")->getChildByName("Panel_Bg_Anim");
    bg->getChildByName("Particle_1")->removeFromParent();
    bg->getChildByName("Particle_2")->removeFromParent();
    
    //layout
    float scale = max((frameSize.height / dSize.height), (frameSize.width / dSize.width));
    
    //bg
    bg->setScale(scale);
    
    //bottom
    Node* current = loading->getChildByName("panel_login")->getChildByName("Panel_Bottom");
    current->setPositionY(current->getPositionY() + (dSize.height - frameSize.height)/2);
}

void BattleLoadingUI::updateProgress(float percent)
{
    auto* loading = this->getChildByName("loginPanel");
    auto* bottom = loading->getChildByName("panel_login")->getChildByName("Panel_Bottom");
    auto* loadingBar = bottom->getChildByName<cocos2d::ui::LoadingBar*>("LoadingBar");
    loadingBar->setPercent(percent);
    auto* progressText = bottom->getChildByName<cocos2d::ui::Text*>("Text_Progress");
    progressText->setString(Value((int)percent).asString() + " %");
}
