//
//  PopupMessageBox2Buttons.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 13/08/2019.
//

#ifndef PopupMessageBox2Buttons_h
#define PopupMessageBox2Buttons_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/CTAButton.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include "PopupMessageBox.h"

NS_AZOOMEE_BEGIN

class PopupMessageBox2Buttons : public PopupMessageBox
{
    typedef PopupMessageBox Super;
private:
    
    CTAButton* _secondActionButton = nullptr;
    ButtonPressedCallback _secondCallback = nullptr;
    
public:
    
    bool init() override;
    
    void setSecondButtonPressedCallback(const ButtonPressedCallback& callback);

    void setSecondButtonText(const std::string& buttonText);
    void setSecondButtonColour(const cocos2d::Color3B& colour);
    
    CREATE_FUNC(PopupMessageBox2Buttons);
};

NS_AZOOMEE_END

#endif /* PopupMessageBox2Buttons_h */
