#ifndef Azoomee_SettingsButton_h
#define Azoomee_SettingsButton_h

#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

using namespace Azoomee;

class SettingsButton : public ElectricDreamsButton
{
    typedef ElectricDreamsButton Super;
private:
    cocos2d::Sprite* buttonSprite;
    
protected:
    virtual bool init() override;
    
    virtual void onButtonPressed() override;
    
public:
    CREATE_FUNC(SettingsButton);
    static SettingsButton* createSettingsButton(float creationDelay);
    
};

#endif
