//
//  BackgroundCreator.cpp
//  Game
//
//  Created by fuchenhao on 3/9/15.
//
//

#include "BackgroundCreator.h"
#include "ProjectConfig.h"

BackgroundCreator::BackgroundCreator(BattleStage* stage)
{
    m_stage = stage;
}

BackgroundCreator::~BackgroundCreator()
{
}

void BackgroundCreator::addBackground(const std::string& floor, const std::string& bg1, const std::string& bg2_1, const std::string& bg2_2, const std::string& front)
{
    cocos2d::Sprite* current;
    std::string path = "png/bg/";
    
    for(int i = 0; i < 4; i++)
    {
        current = cocos2d::Sprite::create(path + bg2_1);
        current->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
        current->setRotation3D(cocos2d::Vec3(90, 0, 0));
        current->setPositionX(-1280 + 2048 * i * 2);
        current->setPositionY(1024 * 1.5 * 2);
        current->setPositionZ(-384);
        current->setScale(2.0);
        m_stage->getBackground()->addChild(current);
        current->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);
    }
    
    for(int i = 0; i < 4; i++)
    {
        current = cocos2d::Sprite::create(path + bg2_2);
        current->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
        current->setRotation3D(cocos2d::Vec3(90, 0, 0));
        current->setPositionX(-1280 + 2048 * i * 2);
        current->setPositionY(1024 * 1.5 * 2);
        current->setPositionZ(-384 + 1024);
        current->setScale(2.0);
        m_stage->getBackground()->addChild(current);
        current->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);
    }
    
    for(int i = 0; i < 6; i++)
    {
        current = cocos2d::Sprite::create(path + floor);
        current->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
        current->setPositionX(-768 + 2048 * 0.7 * i);
        current->setPositionY(-8);
        current->setScaleY(1.6);
        current->setScaleX(0.7);
        m_stage->getBackground()->addChild(current);
        current->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);
    }
    
    for(int i = 0; i < 5; i++)
    {
        current = cocos2d::Sprite::create(path + bg1);
        current->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
        current->setRotation3D(cocos2d::Vec3(90, 0, 0));
        current->setPositionX(-1280 + 2048 * i);
        current->setPositionY(1024 * 1.5);
        current->setPositionZ(0);
        m_stage->getBackground()->addChild(current);
        current->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);
    }
    
    /*
    for(int i = 0; i < 5; i++)
    {
        current = cocos2d::Sprite::create(path + front);
        current->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
        current->setPositionX(-1280 + 2048 * i * 0.5);
        current->setPositionY(100);
        current->setScale(0.5);
        m_stage->getRole()->addChild(current, 0);
        current->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);
    }*/
    
    #if SHOW_GRID
    cocos2d::DrawNode* drawNode = cocos2d::DrawNode::create();
    m_stage->getBackground()->addChild(drawNode);
    drawNode->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);
    
    for (int i = 0; i < STAGE_HEIGHT + 1; i++)
    {
        drawNode->drawLine(cocos2d::Vec2(0, i * GRID_HEIGHT), cocos2d::Vec2(GRID_WIDTH * STAGE_WIDTH, i * GRID_HEIGHT), cocos2d::Color4F(0, 0, 0, 1.0));
    }
    
    for (int i=0; i < STAGE_WIDTH + 1; i++)
    {
        drawNode->drawLine(cocos2d::Vec2(i * GRID_WIDTH, 0), cocos2d::Vec2(i * GRID_WIDTH, GRID_WIDTH * STAGE_HEIGHT), cocos2d::Color4F(0, 0, 0, 1.0));
    }
    
    for (int i=0; i< STAGE_WIDTH ; i++)
    {
        for (int j=0; j<STAGE_HEIGHT; j++)
        {
            cocos2d::Label *labelDamage = cocos2d::Label::createWithBMFont("fonts/NormalNum.fnt", cocos2d::StringUtils::format("%d+%d", i, j));
            labelDamage->setTag(i * 100 + j);
            labelDamage->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
            labelDamage->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);
            labelDamage->setPosition(cocos2d::Vec2(i*GRID_WIDTH+HALF_GRID_WIDTH,j*GRID_HEIGHT+HALF_GRID_HEIGHT));
            m_stage->getBackground()->addChild(labelDamage);
        }
    }
    #endif
}
