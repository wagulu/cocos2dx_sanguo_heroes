//
//  AdvancedAnimation
//
//  Created by fuchenhao on 3/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "CCAdvancedAnimation.h"
#include "CCAssetDefineCache.h"
#include <iostream>

#include "cocos2d.h"
#include <vector>

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


NS_CC_BEGIN

AdvancedAnimation::AdvancedAnimation()
{
    m_currentFrame = 0;
    m_playFlag = true;
    m_elaspeTime = 0;
    m_playOnce = false;
    m_fireLabel = false;
    m_lastLabel = -1;

    m_shaderNormal = GLProgramCache::getInstance()->getGLProgram("animation_shader");
    m_shaderAlpha = GLProgramCache::getInstance()->getGLProgram("animation_shader_alpha");
    setGLProgram(m_shaderNormal);
    
    m_uColorLocation = getGLProgram()->getUniformLocation("u_color");
    
    //this->scheduleUpdate();
    m_replaces = new std::map<int, cocos2d::Node*>();
    
    m_blendMode_src = GL_ONE;//GL_SRC_ALPHA;
    m_blendMode_dst = GL_ONE_MINUS_SRC_ALPHA;
    m_blendType = BLEND_NORMAL;
    
    m_uniformColor = Vec4(1.0, 1.0, 1.0, 1.0);
}

AdvancedAnimation::~AdvancedAnimation()
{
    this->m_define = nullptr;
    m_onComplete = nullptr;
    
    removeAllReplaces();
    m_replaces->clear();
    delete m_replaces;
    
    for (auto* pCmd : m_renderCommands)
    {
        delete pCmd;
    }
    m_renderCommands.clear();
}

AdvancedAnimation* AdvancedAnimation::create(AssetDefine* define, const std::string &animationName, float delayShow)
{
    auto animation = new AdvancedAnimation();
    animation->setDefine(define, animationName, delayShow);
    animation->autorelease();
    return animation;
}

void AdvancedAnimation::setBlendMode(int blend)
{
    switch (blend)
    {
        case BLEND_NORMAL:
            m_blendMode_src = GL_ONE;//GL_SRC_ALPHA;
            m_blendMode_dst = GL_ONE_MINUS_SRC_ALPHA;
            setGLProgram(m_shaderNormal);
            break;
            
        case BLEND_LIGHTEN:
            CCLOG("blending mode LIGHTEN is NOT support !");
            m_blendMode_src = GL_ONE_MINUS_DST_COLOR;
            m_blendMode_dst = GL_ONE;
            setGLProgram(m_shaderAlpha);
            break;
            
        case BLEND_ADD:
            m_blendMode_src = GL_ONE;
            m_blendMode_dst = GL_ONE;
            setGLProgram(m_shaderNormal);
//            m_blendMode_src = GL_SRC_ALPHA;
//            m_blendMode_dst = GL_ONE;
//            setGLProgram(m_shaderAlpha);
            break;
            
            
        default:
            break;
    }
    
    m_blendType = blend;
}

//void AdvancedAnimation::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
//{
//    m_uniformColor = Vec4(r, g, b, a);
//}

//const cocos2d::Vec4& AdvancedAnimation::getColor()
//{
//    return m_uniformColor;
//}

CustomCommand* AdvancedAnimation::getRenderCommand(int index)
{
    unsigned long size = m_renderCommands.size();
    while (size <= index)
    {
        m_renderCommands.push_back(new CustomCommand());
        size++;
    }
    return m_renderCommands[index];
}

void AdvancedAnimation::onDraw(const Mat4 &transform, ssize_t numberOfQuads, ssize_t start)
{
    CC_NODE_DRAW_SETUP();
    
    getGLProgram()->setUniformLocationWith4f(m_uColorLocation, m_uniformColor.x, m_uniformColor.y, m_uniformColor.z, m_uniformColor.w);
    
    glBlendFunc(m_blendMode_src, m_blendMode_dst);
    
    this->m_animation->atlas->drawNumberOfQuads(numberOfQuads, start);
    
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void AdvancedAnimation::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if (m_noDraw && m_delayShow > 0) return;
    
    //ccPosition_uColor_vert
    if(this->m_animation->frames[this->m_currentFrame] > 0)
    {
        if(!m_animation->binded[this->m_currentFrame])
        {
            this->m_animation->atlas->getQuads();
            this->m_animation->binded[this->m_currentFrame] = true;
        }
        
        int cmdIndex = 0;
        CustomCommand *renderCmd;
        
        if(m_replaces->size() == 0)
        {
            renderCmd = getRenderCommand(cmdIndex++);
            renderCmd->init(_globalZOrder, transform, flags);
            renderCmd->func = CC_CALLBACK_0(AdvancedAnimation::onDraw, this, transform, this->m_animation->frames[this->m_currentFrame], this->m_animation->addedFrames[this->m_currentFrame]);
            renderer->addCommand(renderCmd);
        }
        else
        {
            int start = 0;
            int index = 0;
            int totalFrame = this->m_animation->frames[this->m_currentFrame];
            int firstFrame = this->m_animation->addedFrames[this->m_currentFrame];
            std::map<int, cocos2d::Node*>::iterator iter;

            for(int i = 0; i < totalFrame; i++)
            {
                index = this->m_animation->quads->at(firstFrame + i)->index;
                iter = m_replaces->find(index);
                if(iter != m_replaces->end())
                {
                    AssetDefine::QuadData* data = this->m_animation->quads->at(firstFrame + i);
                    Mat4 mat = Mat4(data->a, data->c, 0, data->x, data->b, data->d, 0, -data->y, 0, 0, 1, 0, 0, 0, 0, 1);
                    iter->second->setColor(Color3B(data->color.r, data->color.g, data->color.b));
                    iter->second->setOpacity(data->color.a);
                    iter->second->visit(renderer, transform * mat, true);
                    if(start != i)
                    {
                        renderCmd = getRenderCommand(cmdIndex++);
                        renderCmd->init(_globalZOrder, transform, flags);
                        renderCmd->func = CC_CALLBACK_0(AdvancedAnimation::onDraw, this, transform, (i - start), (firstFrame + start));
                        renderer->addCommand(renderCmd);
                    }
                    start = i + 1;
                }
            }
            if(start != totalFrame)
            {
                renderCmd = getRenderCommand(cmdIndex++);
                renderCmd->init(_globalZOrder, transform, flags);
                renderCmd->func = CC_CALLBACK_0(AdvancedAnimation::onDraw, this, transform, (totalFrame - start), (firstFrame + start));
                renderer->addCommand(renderCmd);
            }
        }
    }
}

void AdvancedAnimation::update(float dt)
{
    if (m_delayShow == 0)
    {
        m_noDraw = false;
    }
    else if (m_delayShow > 0 && m_noDraw)
    {
        m_delayShow -= dt;
        if (m_delayShow <= 0) m_noDraw = false;
        
        return;
    }
    
    //TO-DO update replaced particle
    if(this->m_playFlag)
    {
        this->m_elaspeTime += dt;
        while(this->m_elaspeTime >= this->m_frameInterval)
        {
            this->m_elaspeTime -= this->m_frameInterval;		
            this->m_currentFrame++;
            if(this->m_fireLabel)
            {
                if(this->m_lastLabel + 1 < this->m_animation->labelCount)
                {
                    if(this->m_currentFrame >= this->m_animation->labels->at(this->m_lastLabel + 1)->frame)
                    {
                        if(this->m_onLabel != nullptr)
                        {
                            this->m_onLabel(m_lastLabel + 1);
                        }
                        this->m_lastLabel++;
                    }
                }
            }
        }
        if(this->m_currentFrame > this->m_animation->frameCount - 1)
        {
            m_lastLabel = -1;
            if(this->m_playOnce)
            {
                this->m_playOnce = false;
                this->m_playFlag = false;
                this->m_currentFrame = this->m_animation->frameCount - 1;
                if(this->m_onComplete != nullptr)
                {
                    this->m_onComplete();
                }
                if(m_removeWhenComplete)
                {
                    this->removeFromParent();
                }
            }
            else
            {
                this->m_currentFrame = 0;
            }
        }
    }
}

//int AdvancedAnimation::getTotalFrames()
//{
//    return this->m_animation->frameCount;
//}

void AdvancedAnimation::setDefine(AssetDefine* define, const std::string &animationName, float delayShow)
{
    this->m_define = define;
    this->setCurrentAnimation(animationName);
    this->m_delayShow = delayShow;
}

void AdvancedAnimation::setCurrentAnimation(const std::string &animationName)
{
    auto animation = this->m_define->getAnimation(animationName);
    if(animation == nullptr)
    {
        printf("wrong animation %s\n", animationName.c_str());
        if(!m_animation)
        {
            printf("error animation %s\n", animationName.c_str());
        }
    }
    else
    {
        m_animation = animation;
    }
    this->m_frameInterval = 1.0 / (float)(this->m_define->getFPS());
    this->m_currentFrame = 0;
}

//std::string* AdvancedAnimation::getCurrentAnimation()
//{
//    return this->m_animation->name;
//}

//void AdvancedAnimation::setLabelFire(onLabelChange onChange)
//{
//    this->m_onLabel = onChange;
//    this->m_fireLabel = true;
//    this->m_lastLabel = -1;
//}

//void AdvancedAnimation::removeLabelFire()
//{
//    this->m_fireLabel = false;
//    this->m_onLabel = nullptr;
//}

void AdvancedAnimation::addReplace(const std::string &name, cocos2d::Node* asset)
{
    int index = m_define->getAssetIndex(name);
    if(index != -1)
    {
        asset->retain();
        m_replaces->insert(std::pair<int, cocos2d::Node*>(index, asset));
    }
}

void AdvancedAnimation::removeReplace(const std::string &name)
{
    int index = m_define->getAssetIndex(name);
    if(index != -1)
    {
        m_replaces->erase(index);
    }
}

void AdvancedAnimation::removeAllReplaces()
{
    std::map<int, cocos2d::Node*>::iterator replace_it = m_replaces->begin();
    while(replace_it != m_replaces->end())
    {
        CC_SAFE_RELEASE(replace_it->second);
        replace_it->second = NULL;
        m_replaces->erase(replace_it++);
    }
}

NS_CC_END


//lua binding

static std::vector<cocos2d::AdvancedAnimation*> advanceAnimationList;

int freeAllAdvancedAnimation(lua_State *L)
{
    advanceAnimationList.clear();

    lua_pushinteger(L, 0);
    return 1;
}

int freeAllAdvancedAnimationAsset(lua_State *L)
{
    cocos2d::AssetDefineCache::getInstance()->removeAllAsset();

    lua_pushinteger(L, 0);
    return 1;
}

int createAdvancedAnimation(lua_State *L)
{
    const std::string pathSolder = "png/soldier/";
    const std::string pathHero = "png/hero/";
    const std::string pathSkill = "png/skill/";

    const char* pDefineName = lua_tostring(L, 1);
    const char* pAnimationName = lua_tostring(L, 2);
//    int delayShow = lua_tointeger(L, 2);

    std::string defineName = pDefineName;
    std::string animationName = pAnimationName;
    cocos2d::AssetDefine *define = cocos2d::AssetDefineCache::getInstance()->getAsset(defineName);
    if(define == NULL)
    {
        cocos2d::AssetDefineCache::getInstance()->addAssetWithFile(pathHero + defineName + ".png", pathHero + defineName + ".dat", defineName);
        define = cocos2d::AssetDefineCache::getInstance()->getAsset(defineName);
    }
    cocos2d::AdvancedAnimation* pAnimation = cocos2d::AdvancedAnimation::create(define, animationName, 0);
    pAnimation->scheduleUpdate();

    int handle = advanceAnimationList.size();
    pAnimation->setTag(handle);
    advanceAnimationList.push_back(pAnimation);

    object_to_luaval<cocos2d::Node>(L, "cc.Node", (cocos2d::Node*)pAnimation);

    
    return 1;
}

int setCurrentAnimation(lua_State *L)
{
    int handle = lua_tointeger(L, 1);
    cocos2d::AdvancedAnimation* pAnimation = advanceAnimationList[handle];

    const char* pAnimationName = lua_tostring(L, 2);
    std::string animationName = pAnimationName;

    if(pAnimation)
    {
        pAnimation->setCurrentAnimation(animationName);
    }
    
    return 1;
}

int play(lua_State *L)
{
    int handle = lua_tointeger(L, 1);
    cocos2d::AdvancedAnimation* pAnimation = advanceAnimationList[handle];

    if(pAnimation)
    {
        pAnimation->play();
    }

    return 1;
}

int stop(lua_State *L)
{
    int handle = lua_tointeger(L, 1);
    cocos2d::AdvancedAnimation* pAnimation = advanceAnimationList[handle];
    
    if(pAnimation)
    {
        pAnimation->stop();
    }
    
    return 1;
}

int playOnce(lua_State *L)
{
    int handle = lua_tointeger(L, 1);
    cocos2d::AdvancedAnimation* pAnimation = advanceAnimationList[handle];
    
    if(pAnimation)
    {
        pAnimation->playOnce();
    }

    return 1;
}

int setColor(lua_State *L)
{
    int handle = lua_tointeger(L, 1);
    cocos2d::AdvancedAnimation* pAnimation = advanceAnimationList[handle];

    float r = lua_tonumber(L, 2);
    float g = lua_tonumber(L, 3);
    float b = lua_tonumber(L, 4);
    float a = lua_tonumber(L, 5);

    if(pAnimation)
    {
        pAnimation->setColor(r, g, b, a);
    }
    
    return 1;

}

