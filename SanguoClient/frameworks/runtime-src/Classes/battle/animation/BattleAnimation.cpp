//
//  BattleAnimation.cpp
//  TestAnimation
//
//  Created by fuchenhao on 8/6/14.
//
//

#include "BattleAnimation.h"
#include "BattleWorld.h"

BattleAnimation::BattleAnimation(cocos2d::AdvancedAnimation *animation)
{
    m_animation = animation;
    m_completed = false;
    m_current = -1;
    m_onComplete = std::bind(&BattleAnimation::onAnimationComplete, this);
}

void playCurrent(bool loop);
void playAnimation(int id, bool loop);
void playAnimation(const std::string& animationName, bool loop);


BattleAnimation::~BattleAnimation()
{
    m_animation->removeFromParent();
    //delete m_animation;//?
}



void BattleAnimation::playAnimation(int id, bool loop, bool randomStart)
{
    if(m_current != id || loop == false)
    {
        m_current = id;
        m_animation->setCurrentAnimation(BattleConfig::getInstance()->getAnimationName(m_current));
        m_completed = false;
        
        if(loop)
        {
            if (randomStart)
            {
                m_animation->gotoAndPlay(cocos2d::random() % m_animation->getTotalFrames());
            }
            else
            {
                m_animation->gotoAndPlay(1);
            }
        }
        else
        {
            m_animation->playOnce(m_onComplete);
        }
    }
}

void BattleAnimation::playAnimation(const std::string &animationName, bool loop, bool randomStart)
{
    m_animation->setCurrentAnimation(animationName);
    m_current = -1;
    m_completed = false;
    
    if(loop)
    {
        if (randomStart)
        {
            m_animation->gotoAndPlay(cocos2d::random() % m_animation->getTotalFrames());
        }
        else
        {
            m_animation->gotoAndPlay(1);
        }
    }
    else
    {
        m_animation->playOnce(m_onComplete);
    }
}

void BattleAnimation::playCurrent(bool loop, bool randomStart)
{
    m_completed = false;
    
    if(loop)
    {
        if (randomStart)
        {
            m_animation->gotoAndPlay(cocos2d::random() % m_animation->getTotalFrames());
        }
        else
        {
            m_animation->gotoAndPlay(1);
        }
    }
    else
    {
        m_animation->playOnce(m_onComplete);
    }
}

void BattleAnimation::pauseCurrent(bool pauseFlag)
{
    if (pauseFlag) {
        m_animation->stop();
    } else {
        m_animation->play();
    }
}

