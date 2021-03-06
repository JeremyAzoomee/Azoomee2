#ifndef AzoomeeCommon_AudioMixer_h
#define AzoomeeCommon_AudioMixer_h

#include <cocos/cocos2d.h>

#define OK_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_06_v1.mp3"
#define TEXT_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_07_v1.mp3"
#define CANCEL_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_01_v1.mp3"
#define NEXT_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_06_v1.mp3"
#define BACK_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_01_v1.mp3"
#define ACCEPT_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_06_v1.mp3"
#define SELECT_OOMEE_AUDIO_EFFECT "Azoomee_Button_Click_08_v1.mp3"
#define SETTINGS_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_07_v1.mp3"

#define HQ_ELEMENT_SELECTED_AUDIO_EFFECT "Azoomee_Button_Click_08_v1.mp3"

#define HQ_BACKGROUND_MUSIC "bgmusic.mp3"


namespace Azoomee
{

class AudioMixer : public cocos2d::Ref
{
    
public:
    static AudioMixer* getInstance(void);
    virtual ~AudioMixer();
    bool init(void);
    
    void playBackgroundMusic(const std::string& backgroundMusicToPlay);
    int playEffect(const std::string& effectToPlay);
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
	void stopEffect(int effectId);
	
    void playOomeeEffect(std::string oomee, std::string state, bool fallbackToDefaultSound);
    void playOomeeIdleSounds(bool playSounds);
    void stopOomeeEffect();
    
private:
    int lastOomeeAudio;
    bool shouldPlayOomeeIdleSounds;

};

}

#endif
