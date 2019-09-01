//
//  AssetDefine
//
//  Created by fuchenhao on 3/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "CCAssetDefine.h"
#include "CCBufferReader.h"
#include "CCDirector.h"
#include "platform/CCFileUtils.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCTextureCache.h"
#include "base/ccTypes.h"
#include "cocos2d.h"
#include <zlib.h>

NS_CC_BEGIN

AssetDefine* AssetDefine::create(const std::string &image, const std::string &define)
{
    auto ret = new AssetDefine();
    if (ret && ret->initWithFile(image, define))
    {
        ret->autorelease();
    } else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

AssetDefine::AssetDefine()
{
    
}

bool AssetDefine::initWithFile(const std::string &image, const std::string &define)
{
    ssize_t size;
    auto buff = FileUtils::getInstance()->getFileData(define, "r", &size);
    
    Byte* uncompressed = new Byte[size * 10];
    uLongf destLen = size * 10;
    int result = uncompress(uncompressed, &destLen, buff, size);
    if(result != Z_OK)
    {
        printf("Parse asset data error: %s", define.c_str());
        return false;
    }

    auto reader = new BufferReader(uncompressed, destLen, BIG_ENDIAN_MODE);
    
    //read head
    if(reader->readByte() == 115 && reader->readByte() == 104 && reader->readByte() == 99)
    {
        //read fps
        this->m_fps = reader->readByte();
        
        //read tilesheet
        this->m_texture = Director::getInstance()->getTextureCache()->addImage(image);
        //this->m_texture->generateMipmap();
        this->m_width = reader->readShort();
        this->m_height = reader->readShort();
        this->m_tilesheetCount = reader->readShort();
        this->m_tilesheet = new std::vector<TilesheetData*>();
        
        for(int i = 0; i < m_tilesheetCount; i++)
        {
            TilesheetData *quad = new TilesheetData;
            quad->isRotated = reader->readByte();
            quad->centerX = reader->readFloat();
            quad->centerY = reader->readFloat();
            quad->x = reader->readFloat();
            quad->y = reader->readFloat();
            quad->width = reader->readFloat();
            quad->height = reader->readFloat();
            
            this->m_tilesheet->push_back(quad);
        }
        
        //read asset
        m_assets = new std::vector<std::string*>();
        int assetNum = reader->readShort();
        for(int i = 0; i < assetNum; i++)
        {
            m_assets->push_back(reader->readString());
        }
        
        //read imported
        m_imported = new std::vector<std::string*>();
        int importedNum = reader->readShort();
        for(int i = 0; i < importedNum; i++)
        {
            m_imported->push_back(reader->readString());
        }
        
        //read animation
        m_animations = new std::map<std::string, AnimationData*>();
        int animationNum = reader->readShort();
        
        float x, y, a, b, c, d, cx, cy, tx, ty, tw, th;
        int labelCount, frameNum, assetCount;
        TilesheetData *tile;
        
        for(int i = 0; i < animationNum; i++)
        {
            AnimationData *animation = new AnimationData;

            animation->name = reader->readString();
            animation->labels = new std::vector<LabelData*>();
            animation->quads = new std::vector<QuadData*>();
            
            labelCount = reader->readShort();
            animation->labelCount = labelCount;
            for(int j = 0; j < labelCount; j++)
            {
                LabelData* label = new LabelData;
                label->frame = reader->readShort();
                label->name = reader->readString();
                animation->labels->push_back(label);
            }
            
            frameNum = reader->readShort();
            animation->frameCount = frameNum;
            animation->assetTotal = reader->readInt();

            animation->atlas = TextureAtlas::createWithTexture(this->m_texture, animation->assetTotal);
            animation->atlas->retain();
            //animation->atlas->initWithTexture(this->m_texture, animation->assetTotal);
            animation->atlas->increaseTotalQuadsWith(animation->assetTotal);
            animation->atlas->fillWithEmptyQuadsFromIndex(0, animation->assetTotal);
            
            animation->frames = new int[frameNum];
            animation->addedFrames = new int[frameNum];
            animation->binded = new bool[frameNum];
            
            for(int j = 0; j < frameNum; j++)
            {
                assetCount = reader->readShort();
                animation->frames[j] = assetCount;
                if(j == 0)
                {
                    animation->addedFrames[j] = 0;
                }
                else
                {
                    animation->addedFrames[j] = animation->addedFrames[j - 1] + animation->frames[j - 1];
                }
                animation->binded[j] = false;
                
                for(int k = 0; k < assetCount; k++)
                {
                    //quads index, x, y, a, b, c, d
                    QuadData* data = new QuadData();
                    animation->quads->push_back(data);
                    
                    data->index = reader->readShort();
                    data->x = reader->readFloat();
                    data->y = reader->readFloat();
                    data->a = reader->readFloat();
                    data->b = reader->readFloat();
                    data->c = reader->readFloat();
                    data->d = reader->readFloat();
                    
                    data->color.r = reader->readByte();
                    data->color.g = reader->readByte();
                    data->color.b = reader->readByte();
                    data->color.a = reader->readByte();
                    
                    
                    if(data->color.a != 255)
                    {
                        float alpha = data->color.a / 255.0f;
                        data->color.r *= alpha;
                        data->color.g *= alpha;
                        data->color.b *= alpha;
                    }
                    
                    x = data->x;
                    y = data->y;
                    a = data->a;
                    b = data->b;
                    c = data->c;
                    d = data->d;
                    
                    //CCLOG("quads from: %d", animation->addedFrames[j] + k);
                    V3F_C4B_T2F_Quad* quad = &(animation->atlas->getQuads()[animation->addedFrames[j] + k]);
                    
                    tile = (TilesheetData*)this->m_tilesheet->at(data->index);
                    
                    cx = tile->centerX;
                    cy = tile->centerY;
                    tx = tile->x;
                    ty = tile->y;
                    tw = tile->width;
                    th = tile->height;
                    
                    quad->tl.vertices.x = - a * cx - c * cy + x;
                    quad->tl.vertices.y = + b * cx + d * cy - y;
                    quad->tl.vertices.z = 0.0;
                    quad->tl.texCoords.u = tx / this->m_width;
                    quad->tl.texCoords.v = ty / this->m_height;
                    
                    quad->tr.vertices.x = - a * (cx - tw) - c * cy + x;
                    quad->tr.vertices.y = + b * (cx - tw) + d * cy - y;
                    quad->tr.vertices.z = 0.0;
                    quad->tr.texCoords.u = (tx + tw) / this->m_width;
                    quad->tr.texCoords.v = ty / this->m_height;
                    
                    quad->bl.vertices.x = - a * cx - c * (cy - th) + x;
                    quad->bl.vertices.y = + b * cx + d * (cy - th) - y;
                    quad->bl.vertices.z = 0.0;
                    quad->bl.texCoords.u = tx / this->m_width;
                    quad->bl.texCoords.v = (ty + th) / this->m_height;
                    
                    quad->br.vertices.x = - a * (cx - tw) - c * (cy - th) + x;
                    quad->br.vertices.y = + b * (cx - tw) + d * (cy - th) - y;
                    quad->br.vertices.z = 0.0;
                    quad->br.texCoords.u = (tx + tw) / this->m_width;
                    quad->br.texCoords.v = (ty + th) / this->m_height;
                    
                    quad->bl.colors = data->color;
                    quad->br.colors = data->color;
                    quad->tl.colors = data->color;
                    quad->tr.colors = data->color;
                }
            }
            this->m_animations->insert(std::pair<std::string, AnimationData*>(std::string(animation->name->c_str()), animation));
        }
        
        //read ui
        //TO-DO
        
    } else
    {
        printf("Invalid asset define: %s", define.c_str());
        return false;
    }

    free(buff);
    delete uncompressed;
    delete reader;
    
    return true;
}

AssetDefine::AnimationData* AssetDefine::getAnimation(const std::string &name)
{
    std::map<std::string, AnimationData*>::iterator iter = this->m_animations->find(name);
    
    if(iter != this->m_animations->end())
    {
        return iter->second;
    }
    return nullptr;
}

AssetDefine::~AssetDefine()
{
    printf("release asset define\n");
    
    Director::getInstance()->getTextureCache()->removeTexture(m_texture);
    //CC_SAFE_RELEASE(m_texture);
    
    for (std::vector<std::string*>::iterator it = m_assets->begin(); it != m_assets->end(); it++)
    {
        if (NULL != *it)
        {
            delete *it;
            *it = NULL;
        }
    }
    m_assets->clear();
    delete m_assets;
    
    for (std::vector<std::string*>::iterator it = m_imported->begin(); it != m_imported->end(); it++)
    {
        if (NULL != *it)
        {
            delete *it;
            *it = NULL;
        }
    }
    m_imported->clear();
    delete m_imported;
    
    for (std::vector<TilesheetData*>::iterator it = m_tilesheet->begin(); it != m_tilesheet->end(); it++)
    {
        if (NULL != *it)
        {
            delete *it;
            *it = NULL;
        }
    }
    m_tilesheet->clear();
    delete m_tilesheet;
    
    std::map<std::string, AnimationData*>::iterator animation_it = m_animations->begin();
    while(animation_it != m_animations->end())
    {
        delete animation_it->second;
        animation_it->second = NULL;
        m_animations->erase(animation_it++);
    }
    m_animations->clear();
    delete m_animations;
    
    /*
    std::map<std::string, UIData*>::iterator ui_it = m_ui->begin();
    while(ui_it != m_ui->end())
    {
        delete ui_it->second;
        ui_it->second = NULL;
        m_ui->erase(ui_it++);
    }
    m_ui->clear();
    delete m_ui;*/
}

int AssetDefine::getAssetIndex(const std::string& name)
{
    int total = m_assets->size();
    int index = -1;
    for(int i = 0; i < total; i++)
    {
        if(*m_assets->at(i) == name)
        {
            index = i;
            break;
        }
    }
    return index;
}

cocos2d::Sprite* AssetDefine::createSpriteFrame(const std::string& name)
{
    int index = getAssetIndex(name);

    if(index != -1)
    {
        TilesheetData *quad = this->m_tilesheet->at(index);

        Rect rect = quad->isRotated ? Rect(quad->x, quad->y, quad->height, quad->width) : Rect(quad->x, quad->y, quad->width, quad->height);
        Size size = Size(quad->width, quad->height);
        Vec2 center = quad->isRotated ? Vec2(quad->centerY / quad->height, quad->centerX / quad->width) : Vec2(quad->centerX / quad->width, quad->centerY / quad->height);

        cocos2d::Sprite* s = cocos2d::Sprite::createWithTexture(m_texture);
        s->setTextureRect(rect, quad->isRotated, size);
        s->setAnchorPoint(center);

        return s;
    }
    return nullptr;
}

NS_CC_END
