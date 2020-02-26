//
//  ArtAppConfirmCancelMessageBox.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 20/09/2018.
//

#include "ArtAppConfirmCancelMessageBox.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_ART_BEGIN

ArtAppConfirmCancelMessageBox* ArtAppConfirmCancelMessageBox::createWithParams(const std::string &title, const std::string &confirmButtonFilename, const std::string &cancelButtonFilename, const cocos2d::Color3B &backgroundColour, const cocos2d::Color4B &textColour)
{
    ArtAppConfirmCancelMessageBox* messageBox = ArtAppConfirmCancelMessageBox::create();
    messageBox->setParams(title, confirmButtonFilename, cancelButtonFilename, backgroundColour, textColour);
    return messageBox;
}

bool ArtAppConfirmCancelMessageBox::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorOpacity(0);
    
    _messageBox = ui::Layout::create();
    _messageBox->setBackGroundImage("res/modal/confirm_cancel_popup.png");
    _messageBox->setBackGroundImageScale9Enabled(true);
    _messageBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBox->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _messageBox->setContentSize(Size(890,570));
    this->addChild(_messageBox);
    
    _titleText = Label::createWithTTF("", "fonts/Arial.ttf", 109);
    _titleText->setNormalizedPosition(Vec2(0.5,0.9));
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleText->setTextColor(Color4B::BLACK);
    _messageBox->addChild(_titleText);
    
    _confirmButton = ui::Button::create();
    _confirmButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _confirmButton->setNormalizedPosition(Vec2(0.33,0.4));
    _confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			AudioMixer::getInstance()->playEffect("res/audio/Azoomee_Button_Click_06_v1.mp3");
            if(_confirmCallback)
            {
                _confirmCallback(this);
            }
        }
    });
    _messageBox->addChild(_confirmButton);
    
    _cancelButton = ui::Button::create();
    _cancelButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _cancelButton->setNormalizedPosition(Vec2(0.67,0.4));
    _cancelButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			AudioMixer::getInstance()->playEffect("res/audio/Azoomee_Button_Click_01_v1.mp3");
            if(_cancelCallback)
            {
                _cancelCallback(this);
            }
        }
    });
    _messageBox->addChild(_cancelButton);
    
    return true;
}
void ArtAppConfirmCancelMessageBox::onEnter()
{
    Super::onEnter();
}

void ArtAppConfirmCancelMessageBox::setParams(const std::string& title, const std::string& confirmButtonFilename, const std::string& cancelButtonFilename, const cocos2d::Color3B& backgroundColour, const cocos2d::Color4B& textColour)
{
    _confirmButton->loadTextureNormal(confirmButtonFilename);
    _cancelButton->loadTextureNormal(cancelButtonFilename);
    _messageBox->setBackGroundImageColor(backgroundColour);
    _titleText->setString(title);
    _titleText->setTextColor(textColour);
}

void ArtAppConfirmCancelMessageBox::setOnConfirmCallback(const ButtonPressedCallback& callback)
{
    _confirmCallback = callback;
}

void ArtAppConfirmCancelMessageBox::setOnCancelCallback(const ButtonPressedCallback& callback)
{
    _cancelCallback = callback;
}

NS_AZ_ART_END
