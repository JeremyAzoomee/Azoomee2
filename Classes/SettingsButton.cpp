#include "SettingsButton.h"
#include "ExitOrLogoutLayer.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>


SettingsButton* SettingsButton::createSettingsButton(float creationDelay)
{
    auto button = SettingsButton::create();
    Sprite* buttonSprite = button->buttonSprite;
    
    // Hide button and show it after delay
    buttonSprite->setOpacity(0);
    
    float randomDelay = RandomHelper::random_real(0.2, 0.7);
    buttonSprite->runAction(Sequence::create(DelayTime::create(creationDelay + randomDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    return button;
}

bool SettingsButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    buttonSprite = createSpriteButton("res/navigation/settings.png", SETTINGS_BUTTON_AUDIO_EFFECT);
    addChild(buttonSprite);
    
    mixPanelButtonName = "Settings";
    addListener();
    
    return true;
}

void SettingsButton::onButtonPressed()
{
    ExitOrLogoutLayer::create();
}

