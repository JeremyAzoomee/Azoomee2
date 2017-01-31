
#include "AudioMixer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

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
