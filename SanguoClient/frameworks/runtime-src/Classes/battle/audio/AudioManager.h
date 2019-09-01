//
//  AudioManager.h
//  sanguoClient
//
//  Created by fuchenhao on 4/29/15.
//
//

#ifndef __sanguoClient__AudioManager__
#define __sanguoClient__AudioManager__

#include <string>
#include <vector>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include <functional>
#include "stdlib.h"
#include <map>

#define FADE_IN_STEP     0.5
#define FADE_OUT_STEP    -0.5

#define LOW_MUSIC_VOLUME 0.5

#define MAX_CONCURRENCY  3


class AudioManager
{
public:
    
    enum MUSIC_STATE
    {
        MUSE,
        PLAYING,
        STARTING,
        STOPING,
        PAUSING,
        CHANGING
    };
    
    AudioManager();
    ~AudioManager();
    
    void preloadEffect(const std::string& file);
    
    int playEffect(const std::string& file, bool loop = false, bool isBgMusic = false);
    void stopEffect(int audioId);
    int playEffectWithLowMusicVolume(const std::string& file);
    int playEffectWithLimitedConcurrency(const std::string& file, int maxCount, bool loop);
    
    void playMusic(const std::string& file);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    
    void pauseAllEffect();
    void resumeAllEffect();
    
    void enableEffect();
    void disableEffect();
    void enableMusic();
    void disableMusic();
    
    bool isEffectEnable();
    bool isMusicEnable();
    
    void unloadAllEffects();
    
    void update(float dt);//fade in //fade out for music
    
    static AudioManager* getInstance();
    
private:
    std::string getFilePath(const std::string& fileName);
    
    std::vector<std::string> m_loadedEffects;
    std::vector<int> m_playingEffectsWithLowVolumeMusic;
    std::vector<int> m_playingloopingEffect;
    std::map<std::string, std::vector<int>*> m_playingEffectsWithLimitedConcurrency;
    std::string m_preFolder = "sound/";
    std::string m_currentMusic = "";
    
    bool m_musicEnable = false;//true;
    bool m_effectEnable = false;//true;
    
    float m_currentVolume = 0.0;
    float m_maxVolume = 1.0;
    
    MUSIC_STATE m_state = MUSE;
    
    int m_bgMusicId = -1;
    std::string m_bgMusicPath = "";
    
    static AudioManager* s_sharedAudioManager;
};

extern "C"
{
    int preloadSoundEffect(lua_State *L);
    int playSoundEffect(lua_State *L);
    int playSoundEffectWithLowMusicVolume(lua_State *L);
    
    int playMusic(lua_State *L);
    int stopMusic(lua_State *L);
    int pauseMusic(lua_State *L);
    int resumeMusic(lua_State *L);
    
    int enableMusic(lua_State *L);
    int disableMusic(lua_State *L);
    
    int enableSoundEffect(lua_State *L);
    int disableSoundEffect(lua_State *L);
    
    int unloadAllSoundEffects(lua_State *L);
};

//TO-DO play effect and low down music volumn
//TO-DO play effect and limit concurrency
//TO-DO stop all effects

/*
 bool SimpleAudioEngine::getEffectIsPlaying(unsigned int nSoundId)
 {
 EffectList::iterator p = sharedList().find(nSoundId);
 bool bRet = false;
 if (p != sharedList().end())
 {
 bRet = p->second->IsPlaying();
 }
 return bRet;
 }*/







#endif /* defined(__sanguoClient__AudioManager__) */
