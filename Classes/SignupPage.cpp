//
//  SignupPage.cpp
//  Azoomee
//
//  Created by Macauley on 16/07/2019.
//

#include "SignupPage.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include "Urls.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool SignupPage::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setBackGroundImage("res/onboarding/rounded_rect_45px.png");
	setBackGroundImageScale9Enabled(true);
	
    createInputHolder();
    createProgressIndicator();
    createTermsLinks();
	
	return true;
}
void SignupPage::onEnter()
{
	Super::onEnter();
}
void SignupPage::onExit()
{
	Super::onExit();
}
void SignupPage::onSizeChanged()
{
	Super::onSizeChanged();
	_inputHolder->setPosition((this->getContentSize() / 2.0f) + Size(0,_keyboardOffset));
}

void SignupPage::createInputHolder()
{
    const float elementSpacing = 50.0f;
    
    _inputHolder = ui::Layout::create();
    _inputHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _inputHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _inputHolder->setLayoutType(Type::VERTICAL);
    addChild(_inputHolder);
    
    _topHeading = DynamicText::create("", Style::Font::PoppinsMedium(), 50);
    _topHeading->setTextColor(Color4B(Colours::Color_3B::strongPink));
    _topHeading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,1.5 * elementSpacing)));
    _topHeading->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _topHeading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _topHeading->setTextAreaSize(Size(1000,140));
    _topHeading->setOverflow(Label::Overflow::SHRINK);
    _inputHolder->addChild(_topHeading);
    
    _inputTitle = DynamicText::create("test input title", Style::Font::PoppinsRegular(), 50);
    _inputTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _inputTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _inputTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _inputTitle->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    _inputTitle->setTextAreaSize(Size(1000,140));
    _inputTitle->setOverflow(Label::Overflow::SHRINK);
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
    _continueButton->setTextAreaSizePercent(Vec2(0.9f,0.8f));
    _continueButton->setText(_("Continue"));
    _continueButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
    _continueButton->setTextColour(Color4B::WHITE);
    _continueButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_continueCallback)
            {
                _continueCallback(StringFunctions::trim(_inputBox->getText()));
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
                    _backCallback(StringFunctions::trim(_inputBox->getText()));
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
    
    _inputHolder->setContentSize(Size(0,_topHeading->getContentSize().height + _inputTitle->getContentSize().height + _inputBox->getContentSize().height + _continueButton->getContentSize().height + _backButton->getContentSize().height + (5 * elementSpacing)));
    
}
void SignupPage::createProgressIndicator()
{
    _progressBar = ProgressBar::create();
    _progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _progressBar->setNormalizedPosition(Vec2(0.5,0.15));
    _progressBar->setNumberOfSteps(3);
    _progressBar->setProgress(1);
    _progressBar->setScale(700 / _progressBar->getContentSize().width);
    addChild(_progressBar);
    
    _progressText = DynamicText::create(_("test progress text"), Style::Font::PoppinsRegular(), 50);
    _progressText->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    _progressText->setAnchorPoint(Vec2(0.5f,1.5f));
    _progressText->setNormalizedPosition(Vec2(0.5,0.15));
    addChild(_progressText);
}
void SignupPage::createTermsLinks()
{
    _termsLink = createTermsLinkWithURL(_("Terms of use"),Url::TermsOfUse);
    _termsLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _termsLink->setNormalizedPosition(Vec2(0.1,0.05));
    addChild(_termsLink);
    
    _privacyPolicyLink = createTermsLinkWithURL(_("Privacy Policy"), Url::PrivacyPolicyNoLinks);
    _privacyPolicyLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _privacyPolicyLink->setNormalizedPosition(Vec2(0.9,0.05));
    addChild(_privacyPolicyLink);
}

DynamicText* SignupPage::createTermsLinkWithURL(const std::string& title, const std::string& url)
{
    DynamicText* link = DynamicText::create(title, Style::Font::PoppinsRegular(), 35);
    link->ignoreContentAdaptWithSize(false);
    link->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    link->setTextVerticalAlignment(TextVAlignment::CENTER);
    link->setTextHorizontalAlignment(TextHAlignment::CENTER);
    link->setTouchEnabled(true);
    link->setOverflow(Label::Overflow::SHRINK);
    link->addTouchEventListener([link, url](Ref* pSender, ui::Widget::TouchEventType eType){
        switch(eType)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
            {
                link->setTextColor(Color4B(Colours::Color_3B::strongPink));
                link->setScale(1.1f);
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::MOVED:
            {
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::ENDED:
            {
                link->setTextColor(Color4B(Colours::Color_3B::brownGrey));
                link->setScale(1.0f);
                ModalWebview::createWithURL(url, "res/buttons/windowCloseButton.png");
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
            {
                link->setTextColor(Color4B(Colours::Color_3B::brownGrey));
                link->setScale(1.0f);
                break;
            }
        }
    });
    
    return link;
}

void SignupPage::setContinueCallback(const ButtonCallback& callback)
{
	_continueCallback = callback;
}
void SignupPage::setBackCallback(const ButtonCallback& callback)
{
	_backCallback = callback;
}

void SignupPage::repositionForKeyboardHeight(int height, float duration)
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

float SignupPage::getMoveDistanceForKeyboardHeight(int height)
{
	float offset = height - _backButton->getWorldPosition().y;
	float headingPos = _topHeading->getWorldPosition().y + offset + _topHeading->getContentSize().height;
	float worldPos = this->getWorldPosition().y + this->getContentSize().height - 75;
	if(headingPos > worldPos)
	{
		return headingPos - worldPos;
	}
	return 0;
}

void SignupPage::clearInputText()
{
	_inputBox->setText("");
    editBoxTextChanged(_inputBox, "");
}

void SignupPage::setContinueButtonEnabled(bool enabled)
{
	_continueButton->setTouchEnabled(enabled);
    _continueButton->setColor(enabled ? Colours::Color_3B::darkIndigo : Colours::Color_3B::greyBlue2);
}

//Editbox Delegate Functions
void SignupPage::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	
}
void SignupPage::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	
}
void SignupPage::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action)
{
	
}
void SignupPage::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	
}
void SignupPage::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	
}

NS_AZ_END
