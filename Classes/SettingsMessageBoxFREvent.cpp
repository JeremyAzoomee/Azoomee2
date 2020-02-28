//
//  SettingsMessageBoxFREvent.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/09/2018.
//

#include "SettingsMessageBoxFREvent.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/Colour.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

void SettingsMessageBoxFREvent::onEnter()
{
    Super::onEnter();
    
    _messageBox->setContentSize(Size(_visibleSize.width - 100, _visibleSize.height * 0.5f));
	
	const Size& messageBoxSize = _messageBox->getContentSize();
	
    _closeButton = SettingsMessageBoxButton::create();
    _closeButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::CLOSE);
    _closeButton->loadTextureNormal("res/settings/exit_button_red.png");
    _closeButton->setAnchorPoint(Vec2(-0.5,1.5));
    _closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _closeButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
    _messageBox->addChild(_closeButton);
    
    _heading = Label::createWithTTF("", Style::Font::Medium(), 91);
    _heading->setTextColor(Color4B::BLACK);
    _heading->setHorizontalAlignment(TextHAlignment::CENTER);
    _heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _heading->setPosition(Vec2(messageBoxSize.width * 0.5f, messageBoxSize.height * 0.77f));
	_heading->setOverflow(Label::Overflow::SHRINK);
	_heading->setDimensions(messageBoxSize.width * 0.75f, messageBoxSize.height * 0.1f);
    _messageBox->addChild(_heading);
    
    _subHeading = Label::createWithTTF("", Style::Font::Medium(), 59);
    _subHeading->setTextColor(Color4B::BLACK);
    _subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
    _subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _subHeading->setPosition(Vec2(messageBoxSize.width * 0.5f, messageBoxSize.height * 0.66));
	_subHeading->setOverflow(Label::Overflow::SHRINK);
	_subHeading->setDimensions(messageBoxSize.width * 0.75f, messageBoxSize.height * 0.19f);
    _messageBox->addChild(_subHeading);
    
    _textInputHeading = Label::createWithTTF("", Style::Font::Medium(), 59);
    _textInputHeading->setTextColor(Color4B::BLACK);
    _textInputHeading->setHorizontalAlignment(TextHAlignment::CENTER);
    _textInputHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _textInputHeading->setPosition(Vec2(messageBoxSize.width * 0.5f, messageBoxSize.height * 0.46f));
	_textInputHeading->setOverflow(Label::Overflow::SHRINK);
	_textInputHeading->setDimensions(messageBoxSize.width * 0.75f, messageBoxSize.height * 0.1f);
    _messageBox->addChild(_textInputHeading);
    
    _textInput = TextInputLayer::createSettingsBoxTextInput(1090, INPUT_IS_KIDS_CODE);
    _textInput->setCenterPosition(Vec2(messageBoxSize.width * 0.5f, messageBoxSize.height * 0.32f));
    _textInput->setDelegate(this);
    _messageBox->addChild(_textInput);
    
    _sendButton = SettingsMessageBoxButton::create();
    _sendButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::SEND);
    _sendButton->loadTextureNormal("res/settings/settings_rounded.png");
    _sendButton->setColor(Colours::Color_3B::skyBlue);
    _sendButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sendButton->setPosition(Vec2(messageBoxSize.width * 0.5f, messageBoxSize.height * 0.18f));
    _sendButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
    _sendButton->setOpacity(_textInput->inputIsValid() ? 255 : 155);
    _sendButton->setTouchEnabled(_textInput->inputIsValid());
    _messageBox->addChild(_sendButton);
    
    Label* sendLabel = Label::createWithTTF(_("Send"), Style::Font::Medium(), 75);
    sendLabel->setTextColor(Color4B::WHITE);
    sendLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sendLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _sendButton->addChild(sendLabel);
    
    switch (_type) {
        case EventType::SUCCESS:
        {
            _heading->setString(_("Your friend has been invited!"));
            _subHeading->setString(_("Their parents will need to confirm the friendship"));
            _textInputHeading->setString(_("Invite another friend"));
            break;
        }
        case EventType::FAIL:
        {
            _heading->setString(_("Oops!"));
            _subHeading->setString(_("We didn't recognise that Kid Code."));
            _textInputHeading->setString(_("Try again"));
            break;
        }
    }
}

void SettingsMessageBoxFREvent::setType(AZ::EventType type)
{
    _type = type;
}

std::string SettingsMessageBoxFREvent::getInputString() const
{
    return _textInput->getText();
}

// delegate functions

void SettingsMessageBoxFREvent::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _sendButton->setOpacity(isValid ? 255 : 155);
    _sendButton->setTouchEnabled(isValid);
}
void SettingsMessageBoxFREvent::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer->inputIsValid())
    {
        onButtonPressed(_sendButton, ui::Widget::TouchEventType::ENDED);
    }
    
}
void SettingsMessageBoxFREvent::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}
void SettingsMessageBoxFREvent::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

NS_AZ_END
