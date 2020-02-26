#ifndef AzoomeeCommon_AudioMixer_h
#define AzoomeeCommon_AudioMixer_h

#include <cocos/cocos2d.h>
#include "../Tinizine.h"

NS_TZ_BEGIN

class AudioMixer
{
    
public:
    static AudioMixer* getInstance(void);
    virtual ~AudioMixer();
    
    void playBackgroundMusic(const std::string& backgroundMusicToPlay);
    int playEffect(const std::string& effectToPlay);
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
	void stopEffect(int effectId);
    void stopAllEffects();
};

NS_TZ_END

#endif
