//
//  CCAssetDefineCache
//
//  Created by fuchenhao on 3/4/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "CCAssetDefineCache.h"
#include "CCAdvancedAnimation.h"

using namespace std;

NS_CC_BEGIN

AssetDefineCache* AssetDefineCache::s_sharedAssetDefineCache = nullptr;

AssetDefineCache* AssetDefineCache::getInstance()
{
    if (! s_sharedAssetDefineCache)
    {
        s_sharedAssetDefineCache = new AssetDefineCache();
    }
    
    return s_sharedAssetDefineCache;
}

void AssetDefineCache::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(s_sharedAssetDefineCache);
}

AssetDefineCache::AssetDefineCache()
{
    m_assets = new std::map<std::string, AssetDefine*>();
}

AssetDefineCache::~AssetDefineCache()
{
    CCLOGINFO("deallocing AssetDefineCache: %p", this);
    
    removeAllAsset();
    delete m_assets;
}

void AssetDefineCache::addAsset(AssetDefine *define, const std::string& name)
{
    if(hasAsset(name))
    {
        removeAsset(name);
    }

    define->retain();
    this->m_assets->insert(std::pair<std::string, AssetDefine*>(name, define));
}

void AssetDefineCache::removeAllAsset()
{
    std::map<std::string, AssetDefine*>::iterator it = m_assets->begin();
    while(it != m_assets->end())
    {
        assert(it->second->getReferenceCount() == 1);
        CC_SAFE_RELEASE(it->second);
        it->second = NULL;
        it++;
//        m_assets->erase(it++);
    }
    m_assets->clear();
    
}

void AssetDefineCache::removeAsset(const std::string& name)
{
    if (name.size()==0)
        return;
    
//    getAsset(name)->release();
    auto asset = getAsset(name);
    assert(asset->getReferenceCount() == 1);
    CC_SAFE_RELEASE(asset);
    this->m_assets->erase(name);
}

AssetDefine* AssetDefineCache::getAsset(const std::string& name)
{
    std::map<std::string, AssetDefine*>::iterator iter = this->m_assets->find(name);
    
    if(iter != this->m_assets->end())
    {
        return iter->second;
    }
    return NULL;
}

bool AssetDefineCache::hasAsset(const std::string& name)
{
    auto iter = this->m_assets->find(name);
    if (iter != this->m_assets->end()) return true;
    return false;
}

void AssetDefineCache::addAssetWithFile(const std::string& image, const std::string& define, const std::string& name)
{
    if (!hasAsset(name))
    {
        AssetDefine* asset = AssetDefine::create(image, define);
        addAsset(asset, name);
    }
    else
    {
        CCLOG("duplicate asset %s", name.c_str());
    }
}


NS_CC_END
