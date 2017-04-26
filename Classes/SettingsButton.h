#ifndef Azoomee_SettingsButton_h
#define Azoomee_SettingsButton_h

#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

using namespace Azoomee;

class SettingsButton : public ElectricDreamsButton
{
protected:
    virtual void onButtonPressed() override;
    
public:
    CREATE_FUNC(SettingsButton);
    
};

#endif
