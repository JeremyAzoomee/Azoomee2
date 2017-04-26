#include "SettingsButton.h"
#include "ExitOrLogoutLayer.h"


void SettingsButton::onButtonPressed()
{
    ExitOrLogoutLayer::create();
}

