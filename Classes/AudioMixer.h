#include "cocos2d.h"

#define OK_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_06_v1.mp3"
#define TEXT_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_07_v1.mp3"
#define CANCEL_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_01_v1.mp3"
#define NEXT_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_06_v1.mp3"
#define BACK_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_01_v1.mp3"
#define ACCEPT_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_06_v1.mp3"
#define SELECT_OOMEE_AUDIO_EFFECT "Azoomee_Button_Click_08_v1.mp3"
#define SETTINGS_BUTTON_AUDIO_EFFECT "Azoomee_Button_Click_07_v1.mp3"

#define SWIPE_RIGHT_AUDIO_EFFECT "Azoomee_Content_Swipe_Right.mp3"
#define SWIPE_LEFT_AUDIO_EFFECT "Azoomee_Content_Swipe_Left.mp3"

#define HQ_BIG_SQUARES_APPEAR_AUDIO_EFFECT "Azoomee_Big_Squares_Appear_v1.mp3"
#define HQ_HUB_SELECTED_AUDIO_EFFECT "Azoomee_Content_Swoosh_v1.mp3"
#define HQ_ELEMENT_SELECTED_AUDIO_EFFECT "Azoomee_Button_Click_08_v1.mp3"


#define HQ_BACKGROUND_MUSIC "bgmusic.mp3"

class AudioMixer : public cocos2d::Ref
{
    
public:
    static AudioMixer* getInstance(void);
    virtual ~AudioMixer();
    bool init(void);
    
    void playBackgroundMusic(std::string backgroundMusicToPlay);
    void playEffect(std::string effectToPlay);
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    
    void playOomeeEffect(std::string oomee, std::string state, bool fallbackToDefaultSound);
    void stopOomeeEffect();
    int lastOomeeAudio;
    
};
