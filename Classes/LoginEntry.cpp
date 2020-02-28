//
//  LoginEntry.cpp
//  Azoomee
//
//  Created by Macauley on 23/07/2019.
//

#include "LoginEntry.h"
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/Input/TextInputChecker.h>
#include <TinizineCommon/Utils/StringFunctions.h>
#include <TinizineCommon/Data/AppConfig.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool LoginEntry::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setBackGroundImage("res/onboarding/rounded_rect_45px.png");
	setBackGroundImageScale9Enabled(true);
	
	const float elementSpacing = 50.0f;
	
	_inputHolder = ui::Layout::create();
	_inputHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_inputHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_inputHolder->setLayoutType(Type::VERTICAL);
	addChild(_inputHolder);
	
	_inputTitle = DynamicText::create("", Style::Font::PoppinsRegular(), 50);
	_inputTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_inputTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_inputTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
	_inputTitle->setTextColor(Color4B(Colours::Color_3B::brownGrey));
	_inputHolder->addChild(_inputTitle);
	
	_inputBg = ui::Scale9Sprite::create("res/onboarding/input_rounded_frame.png");
	_inputBg->setColor(Color3B(171, 168, 168));
	_inputBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_inputBg->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_inputBg->setContentSize(Size(1060, _inputBg->getContentSize().height));
	_inputBox = ui::EditBox::create(_inputBg->getContentSize() * 0.8f,"res/onboarding/input_rounded_frame.png");
	_inputBox->setTextHorizontalAlignment(TextHAlignment::LEFT);
	_inputBox->setPlaceHolder(_("Tap here to type").c_str());
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
	
	_continueButton = TextButton::create("res/onboarding/rounded_button.png");
	_continueButton->ignoreContentAdaptWithSize(false);
	_continueButton->setContentSize(Size(700,140));
	_continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_continueButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
	_continueButton->setColor(Colours::Color_3B::darkIndigo);
	_continueButton->setText(_("Continue"));
	_continueButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
	_continueButton->setTextColour(Color4B::WHITE);
	_continueButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_state == LoginEntryState::EMAIL)
			{
				_email = StringFunctions::trim(_inputBox->getText());
				if(_continueCallback)
				{
					_continueCallback(_state);
				}
			}
			else
			{
				_password = StringFunctions::trim(_inputBox->getText());
				if(_loginConfirmCallback)
				{
					_loginConfirmCallback(_email,_password);
				}
			}
		}
	});
	_inputHolder->addChild(_continueButton);
	
	_backButton = DynamicText::create(_("Back"), Style::Font::Bold(), 63);
	_backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_backButton->setContentSize(Size(700,140));
	_backButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
	_backButton->setTextColor(Color4B(Colours::Color_3B::brownGrey));
	_backButton->setTouchEnabled(true);
	_backButton->setTouchScaleChangeEnabled(true);
	_backButton->ignoreContentAdaptWithSize(false);
	_backButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_backButton->setTextVerticalAlignment(TextVAlignment::TOP);
	_backButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_backCallback)
			{
				_backCallback(_state);
			}
		}
	});
	_inputHolder->addChild(_backButton);
	
	_inputHolder->setContentSize(Size(0,_inputTitle->getContentSize().height + _inputBox->getContentSize().height + _continueButton->getContentSize().height + _backButton->getContentSize().height + (3 * elementSpacing)));
	
	_versionNumber = DynamicText::create(AppConfig::getInstance()->getVersionNumberToDisplay(), Style::Font::PoppinsRegular(), 35);
	_versionNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_versionNumber->setNormalizedPosition(Vec2(0.5,0.05));
	_versionNumber->setTextColor(Color4B(Colours::Color_3B::brownGrey));
	_versionNumber->setTextVerticalAlignment(TextVAlignment::CENTER);
	_versionNumber->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_versionNumber->setOverflow(Label::Overflow::SHRINK);
	addChild(_versionNumber);
	
	return true;
}
void LoginEntry::onEnter()
{
	Super::onEnter();
}
void LoginEntry::onExit()
{
	Super::onExit();
}
void LoginEntry::onSizeChanged()
{
	Super::onSizeChanged();
	_inputHolder->setPosition((this->getContentSize() / 2.0f) + Size(0,_keyboardOffset));
}

void LoginEntry::setLoginConfirmCallback(const LoginConfirmCallback& callback)
{
	_loginConfirmCallback = callback;
}
void LoginEntry::setBackButtonCallback(const ButtonPressedCallback& callback)
{
	_backCallback = callback;
}
void LoginEntry::setContinueButtonCallback(const ButtonPressedCallback &callback)
{
	_continueCallback = callback;
}

void LoginEntry::setEmail(const std::string& email)
{
	_email = email;
	if(_state == LoginEntryState::EMAIL && _inputBox)
	{
		_inputBox->setText(_email.c_str());
	}
}

void LoginEntry::repositionForKeyboardHeight(int height, float duration)
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

void LoginEntry::clearInputText()
{
	_inputBox->setText("");
}

void LoginEntry::setContinueButtonEnabled(bool enabled)
{
	_continueButton->setTouchEnabled(enabled);
    _continueButton->setColor(enabled ? Colours::Color_3B::darkIndigo : Colours::Color_3B::greyBlue2);
}

void LoginEntry::setState(const LoginEntryState& state)
{
	_state = state;
	switch(state)
	{
		case LoginEntryState::EMAIL:
		{
			_inputTitle->setString(_("Enter your email address"));
			_inputBox->setMaxLength(100);
			_inputBox->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
			_inputBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);//this is most "normal" flag that it can be set as when switching back from password flag
			_inputBox->setText(_email.c_str());
			break;
		}
		case LoginEntryState::PASSWORD:
		{
			_inputTitle->setString(_("Enter your password"));
			_inputBox->setMaxLength(50);
			_inputBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
			_inputBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
			_inputBox->setText("");
			break;
		}
	}
	setContinueButtonEnabled(isValidInput(_inputBox->getText(), _state));
}

bool LoginEntry::isValidInput(const std::string& text, const LoginEntryState& state)
{
	return _state == LoginEntryState::EMAIL ? TextInputChecker::isValidEmailAddress(text.c_str()) : TextInputChecker::isValidPassword(text.c_str(), TextInputChecker::kMinPasswordEntryLength);
}

//Editbox Delegate Functions
void LoginEntry::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	setContinueButtonEnabled(isValidInput(text, _state));
}
void LoginEntry::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidInput(editBox->getText(), _state));
    if(_state == LoginEntryState::EMAIL)
    {
        _email = StringFunctions::trim(_inputBox->getText());
        if(_continueCallback)
        {
            _continueCallback(_state);
        }
    }
    else
    {
        _password = StringFunctions::trim(_inputBox->getText());
        if(_loginConfirmCallback)
        {
            _loginConfirmCallback(_email,_password);
        }
    }
}

void LoginEntry::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidInput(editBox->getText(), _state));
}
void LoginEntry::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	setContinueButtonEnabled(isValidInput(editBox->getText(), _state));
}

NS_AZ_END

