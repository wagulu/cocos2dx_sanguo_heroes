//
//  AdvancedAnimation
//
//  Created by fuchenhao on 3/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef __CC_ADVANCED_ANIMATION_H__
#define __CC_ADVANCED_ANIMATION_H__

#include <functional>
#include <map>
#include "2d/CCNode.h"
#include "CCAssetDefine.h"
#include "cocos2d.h"

NS_CC_BEGIN

class CC_DLL AdvancedAnimation : public Node
{
public:
    enum BlendMode
    {
        BLEND_NORMAL,
        BLEND_LIGHTEN,
        BLEND_ADD,
    };
    
    typedef std::function<void()> onAnimationComplete;
    typedef std::function<void(int labelId)> onLabelChange;
    
private:
    GLProgram* m_shaderNormal;
    GLProgram* m_shaderAlpha;
    std::vector<CustomCommand*> m_renderCommands;
    AssetDefine* m_define;
    AssetDefine::AnimationData* m_animation = nullptr;
    int m_currentFrame;
    bool m_removeWhenComplete;
    float m_frameInterval;
    float m_elaspeTime;
    bool m_playFlag;
    bool m_playOnce;
    bool m_fireLabel;
    int m_lastLabel;
    int m_blendMode_src;
    int m_blendMode_dst;
    int m_blendType;
    GLuint m_uColorLocation;
    onAnimationComplete m_onComplete;
    onLabelChange m_onLabel;
    std::map<int, cocos2d::Node*> *m_replaces;
    
    float m_delayShow = 0.0f;
    bool m_noDraw = true;
    
    Vec4 m_uniformColor;
    
public:
    AdvancedAnimation();
    ~AdvancedAnimation();
    
    //virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);//bool transformUpdated
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    virtual void update(float dt);
    void onDraw(const Mat4& transform, ssize_t numberOfQuads, ssize_t start);
    CustomCommand* getRenderCommand(int index);
    
    void play()
    {
        this->m_playFlag = true;
        this->m_playOnce = false;
    }
    void stop()
    {
        this->m_playFlag = false;
        this->m_playOnce = false;
    }
    void gotoAndPlay(int frame)
    {
        this->m_playFlag = true;
        this->m_playOnce = false;
        if(frame > 0 && frame <= this->m_animation->frameCount)
        {
            this->m_currentFrame = frame - 1;
        }
    }
	void gotoAndStop(int frame)
    {
        this->m_playFlag = false;
        this->m_playOnce = false;
        if(frame > 0 && frame <= this->m_animation->frameCount)
        {
            this->m_currentFrame = frame - 1;
        }
    }

    //TO-DO gotoPlay&stop by label
    int getTotalFrames()
    {
        return this->m_animation->frameCount;
    }
    void setCurrentFrame(int frame)
    {
        if(frame > 0 && frame <= this->m_animation->frameCount)
        {
            this->m_currentFrame = frame - 1;
        }
    }
    
    void playOnce(bool removeWhenComplete = false)
    {
        this->m_playFlag = true;
        this->m_playOnce = true;
        this->m_currentFrame = 0;
        this->m_removeWhenComplete = removeWhenComplete;
    }
    void playOnce(onAnimationComplete onComplete)
    {
        this->m_onComplete = onComplete;
        this->playOnce();
    }
    
    void nextFrame()
    {
        this->m_playFlag = false;
        this->m_playOnce = false;
        if(this->m_currentFrame < this->m_animation->frameCount - 1)
        {
            this->m_currentFrame++;
        }
    }
    void prevFrame()
    {
        this->m_playFlag = false;
        this->m_playOnce = false;
        if(this->m_currentFrame > 0)
        {
            this->m_currentFrame--;
        }
    }
    
    void setDefine(AssetDefine* define, const std::string &animationName, float delayShow = 0.0f);
    void setCurrentAnimation(const std::string &animationName);
    std::string* getCurrentAnimation()
    {
        return this->m_animation->name;
    }
    
    void setBlendMode(int blend);
    
    void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        m_uniformColor.set(r, g, b, a);
    }
    const cocos2d::Vec4& getColor()
    {
        return m_uniformColor;
    }

    void setLabelFire(onLabelChange onChange)
    {
        this->m_onLabel = onChange;
        this->m_fireLabel = true;
        this->m_lastLabel = -1;
    }
    void removeLabelFire()
    {
        this->m_fireLabel = false;
        this->m_onLabel = nullptr;
    }
    
    void addReplace(const std::string& name, cocos2d::Node* asset);
    void removeReplace(const std::string& name);
    void removeAllReplaces();

    
public:
    static AdvancedAnimation* create(AssetDefine* define, const std::string &animationName, float delayShow = 0.0f);
};

NS_CC_END


//lua binding
extern "C" int freeAllAdvancedAnimation(lua_State *L);
extern "C" int freeAllAdvancedAnimationAsset(lua_State *L);
extern "C" int createAdvancedAnimation(lua_State *L);
extern "C" int setCurrentAnimation(lua_State *L);
extern "C" int play(lua_State *L);
extern "C" int stop(lua_State *L);
extern "C" int playOnce(lua_State *L);
extern "C" int setColor(lua_State *L);



#endif
