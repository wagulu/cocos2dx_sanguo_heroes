//
//  BattleAnimation.h
//  TestAnimation
//
//  Created by fuchenhao on 8/6/14.
//
//

#ifndef __BATTLE_ANIMATION_H__
#define __BATTLE_ANIMATION_H__

#include "stdlib.h"
#include "BattleConfig.h"
#include "../../asset/CCAsset.h"

class BattleAnimation
{
    
public:
    BattleAnimation(cocos2d::AdvancedAnimation *animation);
    ~BattleAnimation();
    
    void playCurrent(bool loop, bool randomStart = true);
    void pauseCurrent(bool pauseFlag);
    void playAnimation(int id, bool loop, bool randomStart = true);
    void playAnimation(const std::string& animationName, bool loop, bool randomStart = true);
    
    inline cocos2d::AdvancedAnimation* getAnimation() {return m_animation;};
    int getCurrent() {return m_current;};
    
//    void setPositionX(float x);
//    void setPositionY(float y);
//    void setPosition(float x, float y, float z);
    
    inline void setPositionX(float x)
    {
        m_animation->setPositionX(x);
    }
    inline void setPositionY(float y)
    {
        m_animation->setPositionY(y);
    }
    inline void setPositionZ(float z)
    {
        m_animation->setPositionZ(-z);
        m_animation->setLocalZOrder(-z + m_biasZ);
    }
    inline void setPosition(float x, float y, float z)
    {
        m_animation->setPositionX(x);
        m_animation->setPositionY(y);
        m_animation->setPositionZ(-z);
        m_animation->setLocalZOrder(-z + m_biasZ);
    }
    

//    void setScale(float scale);
//    void setScaleX(float scaleX);
    inline void setScale(float scale)
    {
        m_animation->setScale(scale);
    }
    inline void setScaleX(float scaleX)
    {
        m_animation->setScaleX(scaleX);
    }
    

//    void setVisible(bool visible);
    inline void setVisible(bool visible)
    {
        m_animation->setVisible(visible);
    }

//    bool isComplete();
    inline bool isComplete()
    {
        return m_completed;
    }
//    void onAnimationComplete();
    inline void onAnimationComplete()
    {
        m_completed = true;
    }
    
//    void setAlpha(GLfloat a);
    
//    void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
//    void setColorR(GLfloat r);
//    void setColorG(GLfloat g);
//    void setColorB(GLfloat b);
//    GLfloat getColorR();
//    GLfloat getColorG();
//    GLfloat getColorB();
    inline void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0)
    {
        m_animation->setColor(r, g, b, a);
    }
    inline void setColorR(GLfloat r)
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        m_animation->setColor(r, color.y, color.z, color.w);
    }
    inline void setColorG(GLfloat g)
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        m_animation->setColor(color.x, g, color.z, color.w);
    }
    inline void setColorB(GLfloat b)
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        m_animation->setColor(color.x, color.y, b, color.w);
    }
    inline GLfloat getColorR()
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        return color.x;
    }
    inline GLfloat getColorG()
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        return color.y;
    }
    inline GLfloat getColorB()
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        return color.z;
    }
    
    
//    GLfloat getAlpha();
    inline GLfloat getAlpha()
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        return color.w;
    }
    inline void setAlpha(GLfloat a)
    {
        const cocos2d::Vec4& color = m_animation->getColor();
        m_animation->setColor(color.x, color.y, color.z, a);
    }
    

    inline void setBiasZ(float biasZ) { m_biasZ = biasZ; };
    inline void update(float dt) { m_animation->update(dt); };
    
private:
    
    cocos2d::AdvancedAnimation *m_animation;
    cocos2d::AdvancedAnimation::onAnimationComplete m_onComplete;
    bool m_completed;
    int m_current;
    float m_biasZ = 0;
};


#endif /* defined(__BATTLE_ANIMATION_H__) */
