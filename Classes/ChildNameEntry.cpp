//
//  ChildNameEntry.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 26/11/2019.
//

#include "ChildNameEntry.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ChildNameEntry::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/onboarding/rounded_rect_45px.png");
    setBackGroundImageScale9Enabled(true);
    
    createInputHolder();
    
    return true;
}
void ChildNameEntry::onEnter()
{
    Super::onEnter();
}
void ChildNameEntry::onExit()
{
    Super::onExit();
}
void ChildNameEntry::onSizeChanged()
{
    Super::onSizeChanged();
    _inputHolder->setPosition((this->getContentSize() / 2.0f) + Size(0,_keyboardOffset));
}

void ChildNameEntry::createInputHolder()
{
    const float elementSpacing = 50.0f;
    
    _inputHolder = ui::Layout::create();
    _inputHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _inputHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _inputHolder->setLayoutType(Type::VERTICAL);
    addChild(_inputHolder);
    
    _inputTitle = DynamicText::create(_("Hi, what should I call you?"), Style::Font::PoppinsBold(), 55);
    _inputTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _inputTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _inputTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _inputTitle->setTextColor(Color4B(Style::Color::black));
    _inputTitle->setTextAreaSize(Size(500,180));
    _inputTitle->setOverflow(Label::Overflow::SHRINK);
    _inputHolder->addChild(_inputTitle);
    
    _inputBg = ui::Scale9Sprite::create("res/onboarding/input_rounded_frame.png");
    _inputBg->setColor(Color3B(171, 168, 168));
    _inputBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _inputBg->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _inputBg->setContentSize(Size(1060, _inputBg->getContentSize().height));
    _inputBox = ui::EditBox::create(_inputBg->getContentSize() * 0.8f,"res/onboarding/input_rounded_frame.png");
    _inputBox->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _inputBox->setPlaceHolder(_("Type your name here").c_str());
    _inputBox->setPlaceholderFontName(Style::Font::InputSystemName.c_str());
    _inputBox->setPlaceholderFontColor(Color3B(196,196,196));
    _inputBox->setPlaceholderFontSize(50);
    _inputBox->setFont(Style::Font::InputSystemName.c_str(), 50);
    _inputBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _inputBox->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
    _inputBox->setFontColor(Color3B::BLACK);
    _inputBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _inputBox->setDelegate(this);
    _inputBox->addChild(_inputBg);
    _inputBox->setShouldMoveContentOnKeyboardDisplay(false);
    _inputHolder->addChild(_inputBox);
    
    _continueButton = CTAButton::create("res/onboarding/rounded_button.png");
    _continueButton->ignoreContentAdaptWithSize(false);
    _continueButton->setContentSize(Size(700,140));
    _continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _continueButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
    _continueButton->setColor(Style::Color::darkIndigo);
    _continueButton->setTextAreaSizePercent(Vec2(0.9f,0.8f));
    _continueButton->setText(_("Continue"));
    _continueButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
    _continueButton->setTextColour(Color4B::WHITE);
    _continueButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_continueCallback)
            {
                _continueCallback(trim(_inputBox->getText()));
            }
        }
    });
    _inputHolder->addChild(_continueButton);
    
    _backButton = DynamicText::create(_("Back"), Style::Font::Bold(), 63);
    _backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _backButton->setContentSize(Size(700,140));
    _backButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
    _backButton->setTextColor(Color4B(Style::Color::brownGrey));
    _backButton->setTouchEnabled(true);
    _backButton->setTouchScaleChangeEnabled(true);
    _backButton->ignoreContentAdaptWithSize(false);
    _backButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _backButton->setTextVerticalAlignment(TextVAlignment::TOP);
    _backButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        switch(eType)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
            {
                _backButton->setScale(1.05f);
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::MOVED:
            {
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::ENDED:
            {
                _backButton->setScale(1.0f);
                if(_backCallback)
                {
                    _backCallback(trim(_inputBox->getText()));
                }
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
            {
                _backButton->setScale(1.0f);
                break;
            }
        }
    });
    _inputHolder->addChild(_backButton);
    
    _inputHolder->setContentSize(Size(0, _inputTitle->getContentSize().height + _inputBox->getContentSize().height + _continueButton->getContentSize().height + _backButton->getContentSize().height + (3 * elementSpacing)));
    
}


void ChildNameEntry::setContinueCallback(const ButtonCallback& callback)
{
    _continueCallback = callback;
}
void ChildNameEntry::setBackCallback(const ButtonCallback& callback)
{
    _backCallback = callback;
}

void ChildNameEntry::repositionForKeyboardHeight(int height, float duration)
{
    if(_backButton->getWorldPosition().y < height)
    {
        _keyboardOffset = height - _backButton->getWorldPosition().y;
        _inputHolder->runAction(MoveBy::create(duration, Vec2(0,_keyboardOffset)));
    }
    else if(height == 0)
    {
        _inputHolder->runAction(MoveBy::create(duration, Vec2(0,-_keyboardOffset)));
        _keyboardOffset = 0;
    }
}

float ChildNameEntry::getMoveDistanceForKeyboardHeight(int height)
{
    float offset = height - _backButton->getWorldPosition().y;
    float headingPos = _inputTitle->getWorldPosition().y + offset + _inputTitle->getContentSize().height;
    float worldPos = this->getWorldPosition().y + this->getContentSize().height - 75;
    if(headingPos > worldPos)
    {
        return headingPos - worldPos;
    }
    return 0;
}

void ChildNameEntry::clearInputText()
{
    _inputBox->setText("");
    editBoxTextChanged(_inputBox, "");
}

void ChildNameEntry::setContinueButtonEnabled(bool enabled)
{
    _continueButton->setTouchEnabled(enabled);
    _continueButton->setColor(enabled ? Style::Color::darkIndigo : Style::Color::greyBlue2);
}

void ChildNameEntry::setChildName(const std::string& name)
{
    _inputBox->setText(name.c_str());
    editBoxTextChanged(_inputBox, name.c_str());
}

//Editbox Delegate Functions
void ChildNameEntry::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
    setContinueButtonEnabled(isValidChildName(text.c_str()));
}
void ChildNameEntry::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    setContinueButtonEnabled(isValidChildName(editBox->getText()));
}
void ChildNameEntry::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action)
{
    
}
void ChildNameEntry::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    setContinueButtonEnabled(isValidChildName(editBox->getText()));
}
void ChildNameEntry::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    setContinueButtonEnabled(isValidChildName(editBox->getText()));
}


NS_AZOOMEE_END
