#include "AudioMixer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;


namespace Azoomee
{

static AudioMixer *_sharedConfigStorage = NULL;

AudioMixer* AudioMixer::getInstance()
{
    if (! _sharedConfigStorage)
    {
        _sharedConfigStorage = new AudioMixer();
        _sharedConfigStorage->init();
    }
    
    return _sharedConfigStorage;
}

AudioMixer::~AudioMixer(void)
{
}

bool AudioMixer::init(void)
{
    shouldPlayOomeeIdleSounds = true;
    return true;
}

void AudioMixer::playBackgroundMusic(std::string backgroundMusicToPlay)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(StringUtils::format("res/audio/%s",backgroundMusicToPlay.c_str()).c_str(), true);
}

void AudioMixer::playEffect(std::string effectToPlay)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("res/audio/%s",effectToPlay.c_str()).c_str());

}

void AudioMixer::playOomeeEffect(std::string oomee, std::string state, bool fallbackToDefaultSound)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(lastOomeeAudio);
    
    std::string fileName = oomee + "_" + state + ".mp3";
    std::string fullPath = "res/audio/oomees/" + fileName;
    if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->fullPathForFilename(fullPath)) && shouldPlayOomeeIdleSounds)
        lastOomeeAudio = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(fullPath.c_str());
    else if(fallbackToDefaultSound && shouldPlayOomeeIdleSounds)
        lastOomeeAudio = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/audio/Azoomee_Button_Click_07_v1.mp3");
    
    CCLOG("fullpath: %s", fullPath.c_str());
}

void AudioMixer::playOomeeIdleSounds(bool playSounds)
{
    shouldPlayOomeeIdleSounds = playSounds;
}

void AudioMixer::stopOomeeEffect()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(lastOomeeAudio);
}

void AudioMixer::stopBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void AudioMixer::pauseBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioMixer::resumeBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
  
}
