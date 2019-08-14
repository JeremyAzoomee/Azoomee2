//
//  PopupMessageBox2Buttons.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 13/08/2019.
//

#include "PopupMessageBox2Buttons.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool PopupMessageBox2Buttons::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _actionButton->setNormalizedPosition(Vec2(0.25f,0.3f));
    _actionButton->setContentSize(Size(500,140));
    
    _secondActionButton = CTAButton::create("res/onboarding/rounded_button.png");
    _secondActionButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _secondActionButton->setNormalizedPosition(Vec2(0.75f,0.3f));
    _secondActionButton->ignoreContentAdaptWithSize(false);
    _secondActionButton->setScale9Enabled(true);
    _secondActionButton->setContentSize(Size(500,140));
    _secondActionButton->setColor(Style::Color::darkIndigo);
    _secondActionButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
    _secondActionButton->setTextColour(Color4B::WHITE);
    _secondActionButton->setTextAreaSizePercent(Vec2(0.8f,0.8f));
    _secondActionButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_secondCallback)
            {
                _secondCallback(this);
            }
        }
    });
    _contentBody->addChild(_secondActionButton);
    
    return true;
}

void PopupMessageBox2Buttons::setSecondButtonPressedCallback(const ButtonPressedCallback& callback)
{
    _secondCallback = callback;
}

void PopupMessageBox2Buttons::setSecondButtonText(const std::string& buttonText)
{
    _secondActionButton->setText(buttonText);
}
void PopupMessageBox2Buttons::setSecondButtonColour(const cocos2d::Color3B& colour)
{
    _secondActionButton->setColor(colour);
}


NS_AZOOMEE_END
