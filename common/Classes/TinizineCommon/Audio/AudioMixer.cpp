#include "AudioMixer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;


NS_TZ_BEGIN

static AudioMixer* sAudioMixerSharedInstance = NULL;

AudioMixer* AudioMixer::getInstance()
{
    if (!sAudioMixerSharedInstance)
    {
        sAudioMixerSharedInstance = new AudioMixer();
    }
    
    return sAudioMixerSharedInstance;
}

AudioMixer::~AudioMixer(void)
{
}

void AudioMixer::playBackgroundMusic(const std::string& backgroundMusicToPlay)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(backgroundMusicToPlay.c_str(), true);
}
//returns playing effect num, in case initiater wants to stop the sound
int AudioMixer::playEffect(const std::string& effectToPlay)
{
    return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectToPlay.c_str());

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

void AudioMixer::stopEffect(int effectId)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(effectId);
}

void AudioMixer::stopAllEffects()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

NS_TZ_END
