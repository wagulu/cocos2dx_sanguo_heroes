//
//  AudioManager.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/29/15.
//
//

#include "AudioManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

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

#include "AudioEngine.h"

AudioManager* AudioManager::s_sharedAudioManager = nullptr;

AudioManager::AudioManager()
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->scheduleUpdate(this, 0, false);
}

AudioManager::~AudioManager()
{
    unloadAllEffects();
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->unscheduleUpdate(this);
    
    for(auto it : m_playingEffectsWithLimitedConcurrency)
    {
        delete it.second;
    }
    m_playingEffectsWithLimitedConcurrency.clear();
    
}

AudioManager* AudioManager::getInstance()
{
    if(!s_sharedAudioManager) s_sharedAudioManager = new AudioManager();
    return s_sharedAudioManager;
}

void AudioManager::preloadEffect(const std::string& file)
{
    if(!m_effectEnable) return;
    // audio engine has no preload feature
    // auto path = m_preFolder + file + ".mp3";
    auto path = getFilePath(file);
    if(std::find(m_loadedEffects.begin(), m_loadedEffects.end(), path) == m_loadedEffects.end())
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(path.c_str());
        m_loadedEffects.push_back(path);
    }
}

void AudioManager::unloadAllEffects()
{
    for(auto path : m_loadedEffects)
    {
        cocos2d::experimental::AudioEngine::uncache(path);
        m_loadedEffects.clear();
    }
}

int AudioManager::playEffect(const std::string& file, bool loop, bool isBgMusic)
{
    if(!m_effectEnable) return -1;
    
    if((!isBgMusic) && m_playingloopingEffect.size() >= MAX_CONCURRENCY) return -1;
    
    auto path = getFilePath(file);

    if(!isBgMusic)
    {
        if(std::find(m_loadedEffects.begin(), m_loadedEffects.end(), path) == m_loadedEffects.end())
        {
            m_loadedEffects.push_back(path);
        }
    }
    
    auto id = cocos2d::experimental::AudioEngine::play2d(path, loop);
    
    if(!isBgMusic)
    {
        m_playingloopingEffect.push_back(id);
    }

    log("play effect::");
    //log(path.c_str());
    
    return id;
}

void AudioManager::stopEffect(int audioId)
{
    if(audioId < 0) return;
    
    //AudioState //ERROR  = -1,INITIALZING,PLAYING,PAUSED
    auto state = cocos2d::experimental::AudioEngine::getState(audioId);
    
    if(state != cocos2d::experimental::AudioEngine::AudioState::ERROR)
    {
        cocos2d::experimental::AudioEngine::stop(audioId);
    }
}

int AudioManager::playEffectWithLimitedConcurrency(const std::string& file, int maxCount, bool loop)
{
    if(!m_effectEnable) return -1;
    
    //add total limited
    if(m_playingEffectsWithLimitedConcurrency[file] == NULL)
    {
        m_playingEffectsWithLimitedConcurrency[file] = new std::vector<int>();
    }
    std::vector<int>* list = m_playingEffectsWithLimitedConcurrency[file];

    if(maxCount == 0 ||
       list->size() < maxCount)
    {
        int id = playEffect(file, loop);
        if(id >= 0)
        {
            list->push_back(id);
        }

        return id;
    }
    

    return -1;
}

int AudioManager::playEffectWithLowMusicVolume(const std::string& file)
{
    if(!m_effectEnable) return -1;
    
    if(m_playingEffectsWithLowVolumeMusic.size() == 0)
    {
        m_maxVolume = LOW_MUSIC_VOLUME;
        if(m_bgMusicId >=0 ) cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
    }
    auto id = playEffect(file);
    m_playingEffectsWithLowVolumeMusic.push_back(id);
    return id;
}

void AudioManager::playMusic(const std::string& file)
{
    if(!m_musicEnable) return;
    
    if(m_currentMusic != file)
    {
        m_currentMusic = file;
        if(!m_musicEnable) return;
        
        if(m_state == AudioManager::MUSE)
        {
            m_state = AudioManager::STARTING;
//            auto path = m_preFolder + file + ".mp3";
            auto path = getFilePath(file);
            m_bgMusicPath = path;
            m_bgMusicId = playEffect(file, true);
            cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
        }
        else
        {
            m_state = AudioManager::CHANGING;
        }
    }
    else
    {
        resumeMusic();
    }
}

void AudioManager::pauseAllEffect()
{
    for(auto id : m_playingloopingEffect)
    {
        cocos2d::experimental::AudioEngine::pause(id);
    }
}

void AudioManager::resumeAllEffect()
{
    for(auto id : m_playingloopingEffect)
    {
        cocos2d::experimental::AudioEngine::resume(id);
    }
}

void AudioManager::stopMusic()
{
    if(!m_musicEnable) return;
    
    if(m_state != AudioManager::MUSE)
    {
        m_state = AudioManager::STOPING;
    }
}

void AudioManager::pauseMusic()
{
    if(!m_musicEnable) return;
    
    if(m_state != AudioManager::MUSE && m_currentMusic != "")
    {
        m_state = AudioManager::PAUSING;
    }
}

void AudioManager::resumeMusic()
{
    if(!m_musicEnable) return;
    
    if(m_state != AudioManager::PLAYING && m_currentMusic != "")
    {
        m_state = AudioManager::STARTING;
        if(m_bgMusicId >= 0) cocos2d::experimental::AudioEngine::resume(m_bgMusicId);
    }
}

void AudioManager::enableEffect()
{
    return;
    m_effectEnable = true;
}

void AudioManager::disableEffect()
{
    return;
    m_effectEnable = false;
    //TO-DO stop all playing effect
}

void AudioManager::enableMusic()
{
    return;
    m_musicEnable = true;
    
    resumeMusic();
}

void AudioManager::disableMusic()
{
    return;
    pauseMusic();
    
    m_musicEnable = false;
}

bool AudioManager::isEffectEnable()
{
    return m_effectEnable;
}

bool AudioManager::isMusicEnable()
{
    return m_musicEnable;
}


void AudioManager::update(float dt)
{
    switch(m_state)
    {
        case AudioManager::MUSE:
            break;
            
        case AudioManager::PLAYING:
            break;
            
        case AudioManager::STARTING:
            m_currentVolume += dt * FADE_IN_STEP;
            if(m_currentVolume >= 1)
            {
                m_currentVolume = 1;
                m_state = AudioManager::PLAYING;
            }
            cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
            break;
            
        case AudioManager::STOPING:
            m_currentVolume += dt * FADE_OUT_STEP;
            if(m_currentVolume <= 0.0)
            {
                m_currentVolume = 0.0;
                m_state = AudioManager::MUSE;
                cocos2d::experimental::AudioEngine::stop(m_bgMusicId);
                cocos2d::experimental::AudioEngine::uncache(m_bgMusicPath);
                m_currentMusic = "";
            }
            cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
            break;
            
        case AudioManager::PAUSING:
            m_currentVolume += dt * FADE_OUT_STEP;
            if(m_currentVolume <= 0.0)
            {
                m_currentVolume = 0.0;
                m_state = AudioManager::MUSE;
                cocos2d::experimental::AudioEngine::pause(m_bgMusicId);
            }
            cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
            break;
            
        case AudioManager::CHANGING:
            m_currentVolume += dt * FADE_OUT_STEP;
            if(m_currentVolume <= 0.0)
            {
                m_currentVolume = 0.0;
                m_state = AudioManager::STARTING;
                cocos2d::experimental::AudioEngine::stop(m_bgMusicId);
                cocos2d::experimental::AudioEngine::uncache(m_bgMusicPath);
//                auto path = m_preFolder + m_currentMusic + ".mp3";
                auto path = getFilePath(m_currentMusic);
                m_bgMusicPath = path;
                m_bgMusicId = playEffect(m_currentMusic, true);
                cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
            }
            cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
            break;
    }
    
    //check sound effect
    bool hasEffectsWithLowVolumeMusic = m_playingEffectsWithLowVolumeMusic.size() > 0;
    
    for(auto it = m_playingEffectsWithLowVolumeMusic.begin(); it != m_playingEffectsWithLowVolumeMusic.end();)
    {
        auto current = *it;
        
        //AudioState //ERROR  = -1,INITIALZING,PLAYING,PAUSED
        auto state = cocos2d::experimental::AudioEngine::getState(current);
        
        if(state == cocos2d::experimental::AudioEngine::AudioState::ERROR)
        {
            it = m_playingEffectsWithLowVolumeMusic.erase(it);
        }
        else
        {
            it++;
        }
    }
  
    if(hasEffectsWithLowVolumeMusic && m_playingEffectsWithLowVolumeMusic.size() == 0)
    {
        m_maxVolume = 1.0;
        if(m_bgMusicId >=0 ) cocos2d::experimental::AudioEngine::setVolume(m_bgMusicId, m_currentVolume * m_maxVolume);
    }
    
    for(auto it = m_playingloopingEffect.begin(); it != m_playingloopingEffect.end();)
    {
        auto current = *it;
        
        //AudioState //ERROR  = -1,INITIALZING,PLAYING,PAUSED
        auto state = cocos2d::experimental::AudioEngine::getState(current);
        
        if(state == cocos2d::experimental::AudioEngine::AudioState::ERROR)
        {
            it = m_playingloopingEffect.erase(it);
        }
        else
        {
            it++;
        }
    }
    
    for(auto it : m_playingEffectsWithLimitedConcurrency)
    {
        std::vector<int>* pList = it.second;
        for(auto current = pList->begin(); current != pList->end();)
        {
            auto state = cocos2d::experimental::AudioEngine::getState(*current);
            if(state == cocos2d::experimental::AudioEngine::AudioState::ERROR)
            {
                current = pList->erase(current);
            }
            else
            {
                ++current;
            }
        }

    }
}

int preloadSoundEffect(lua_State *L)
{
    const char* str = lua_tostring(L, 1);
    AudioManager::getInstance()->preloadEffect(std::string(str));
    return 1;
}

int playSoundEffect(lua_State *L)
{
    const char* str = lua_tostring(L, 1);
    AudioManager::getInstance()->playEffect(std::string(str));
    return 1;
}

int playSoundEffectWithLowMusicVolume(lua_State *L)
{
    const char* str = lua_tostring(L, 1);
    AudioManager::getInstance()->playEffectWithLowMusicVolume(std::string(str));
    return 1;
}

int playMusic(lua_State *L)
{
    const char* str = lua_tostring(L, 1);
    AudioManager::getInstance()->playMusic(std::string(str));
    return 1;
}

int stopMusic(lua_State *L)
{
    AudioManager::getInstance()->stopMusic();

    lua_pushinteger(L, 0);
    return 1;
}

int pauseMusic(lua_State *L)
{
    AudioManager::getInstance()->pauseMusic();

    lua_pushinteger(L, 0);
    return 1;
}

int resumeMusic(lua_State *L)
{
    AudioManager::getInstance()->resumeMusic();
    
    lua_pushinteger(L, 0);
    return 1;
}

int enableMusic(lua_State *L)
{
    AudioManager::getInstance()->enableMusic();
    
    lua_pushinteger(L, 0);
    return 1;
}

int disableMusic(lua_State *L)
{
    AudioManager::getInstance()->disableMusic();
    
    lua_pushinteger(L, 0);
    return 1;
}

int enableSoundEffect(lua_State *L)
{
    AudioManager::getInstance()->enableEffect();
    
    lua_pushinteger(L, 0);
    return 1;
}

int disableSoundEffect(lua_State *L)
{
    AudioManager::getInstance()->disableMusic();
    
    lua_pushinteger(L, 0);
    return 1;
}

int unloadAllSoundEffects(lua_State *L)
{
    AudioManager::getInstance()->unloadAllEffects();
    
    lua_pushinteger(L, 0);
    return 1;
}


std::string AudioManager::getFilePath(const std::string& fileName)
{
    auto path = m_preFolder + fileName;
    
    if(fileName.find(".mp3") == std::string::npos &&
       fileName.find(".wav") == std::string::npos)
    {
        path += ".mp3";
    }
    return path;
}



























