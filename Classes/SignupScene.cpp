//
//  SignupScene.cpp
//  Azoomee
//
//  Created by Macauley on 15/07/2019.
//

#include "SignupScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "SignupEnterEmail.h"
#include "SignupConfirmEmail.h"
#include "SignupEnterPassword.h"
#include "SignupEnterPin.h"
#include "SignupTermsPage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string SignupScene::kEnterEmailPageKey = "enterEmail";
const std::string SignupScene::kConfirmEmailPageKey = "confirmEmail";
const std::string SignupScene::kEnterPasswordPageKey = "enterPassword";
const std::string SignupScene::kEnterPinPageKey = "enterPin";
const std::string SignupScene::kTermsPageKey = "terms";

bool SignupScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	
	const Color3B& bgColour = Style::Color::darkIndigo;
	
	_bgColour = ui::Layout::create();
	_bgColour->setBackGroundColorType(ui::HBox::BackGroundColorType::SOLID);
	_bgColour->setBackGroundColor(bgColour);
	_bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bgColour->setSizeType(ui::Layout::SizeType::PERCENT);
	_bgColour->setSizePercent(Vec2(1.0,1.0));
	addChild(_bgColour);
	
	_bgPattern = Sprite::create("res/decoration/main_pattern_big.png");
	_bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_bgPattern->setColor(Style::Color::macaroniAndCheese);
	addChild(_bgPattern);
	
	_patternHider = DrawNode::create();
	if(isPortrait)
	{
		Vec2 points[4] = {Vec2(0,0), Vec2(contentSize.width, 0), Vec2(contentSize.width, contentSize.height * 0.66f), Vec2(0,contentSize.height * 0.66f)};
		_patternHider->drawSolidPoly(points, 4, Color4F(Style::Color::darkIndigo));
	}
	else
	{
		Vec2 points[3] = {Vec2(contentSize.width, 0), Vec2(contentSize.width, contentSize.height), Vec2(0,contentSize.height)};
		_patternHider->drawSolidPoly(points, 3, Color4F(Style::Color::darkIndigo));
	}
	addChild(_patternHider);
	
	_gradient = LayerGradient::create( Color4B(bgColour.r, bgColour.g, bgColour.b, 0), Color4B(bgColour.r, bgColour.g, bgColour.b, isPortrait ? 166 : 245), isPortrait ? Vec2(0.0f, -1.0f) : Vec2(0.0f, 1.0f));
	_gradient->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE);
	_gradient->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_TOP);
	_gradient->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(Vec2(contentSize).distance(Vec2(0,0)),Vec2(contentSize).distance(Vec2(0,0)) / 2.0f));
	_gradient->setIgnoreAnchorPointForPosition(false);
	_gradient->setRotation(CC_RADIANS_TO_DEGREES(isPortrait ? 0 : Vec2(contentSize).getAngle()));
	addChild(_gradient);
	
	_titleText = ui::Text::create("Create your family account", Style::Font::Bold(), 105);
	_titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_titleText->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.85f) : Vec2(0.25f,0.5f));
	_titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
	_titleText->setTextAreaSize(Size(contentSize.width * (isPortrait ? 0.7f : 0.35f),contentSize.height * 0.28f));
	_titleText->setTextColor(Color4B::WHITE);
	addChild(_titleText);
	
	//_signupPage = SignupEnterEmail::create();
	//auto page = SignupConfirmEmail::create();
	//page->setEmail("test@test.com");
	//_signupPage = page;
	//_signupPage = SignupEnterPassword::create();
	/*_signupPage = SignupEnterPin::create();
	_signupPage->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	_signupPage->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	_signupPage->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	_signupPage->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	addChild(_signupPage);*/
	
	//_termsPage = SignupTermsPage::create();
	//_termsPage->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	//_termsPage->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE);
	//_termsPage->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	//_termsPage->setSizePercent(isPortrait ? Vec2(1.0f,0.71f) : Vec2(0.5f, 1.0f));
	//addChild(_termsPage);
	
	SignupEnterEmail* enterEmail = SignupEnterEmail::create();
	enterEmail->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	enterEmail->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	enterEmail->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	enterEmail->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	enterEmail->setVisible(false);
	_pages.insert(kEnterEmailPageKey, enterEmail);
	addChild(enterEmail);
	
	SignupConfirmEmail* confirmEmail = SignupConfirmEmail::create();
	confirmEmail->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	confirmEmail->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	confirmEmail->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	confirmEmail->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	confirmEmail->setVisible(false);
	_pages.insert(kConfirmEmailPageKey, confirmEmail);
	addChild(confirmEmail);
	
	SignupEnterPassword* enterPassword = SignupEnterPassword::create();
	enterPassword->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	enterPassword->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	enterPassword->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	enterPassword->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	enterPassword->setVisible(false);
	_pages.insert(kEnterPasswordPageKey, enterPassword);
	addChild(enterPassword);
	
	SignupEnterPin* enterPin = SignupEnterPin::create();
	enterPin->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	enterPin->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	enterPin->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	enterPin->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	enterPin->setVisible(false);
	_pages.insert(kEnterPinPageKey, enterPin);
	addChild(enterPin);
	
	SignupTermsPage* termsPage = SignupTermsPage::create();
	termsPage->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	termsPage->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE);
	termsPage->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	termsPage->setSizePercent(isPortrait ? Vec2(1.0f,0.71f) : Vec2(0.5f, 1.0f));
	termsPage->setVisible(false);
	_pages.insert(kTermsPageKey, termsPage);
	addChild(termsPage);
	
	changeToPage(kEnterEmailPageKey);
	
	return true;
}
void SignupScene::onEnter()
{
	Super::onEnter();
}
void SignupScene::onExit()
{
	Super::onExit();
}
void SignupScene::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	
	_patternHider->clear();
	if(isPortrait)
	{
		Vec2 points[4] = {Vec2(0,-1), Vec2(contentSize.width, -1), Vec2(contentSize.width, contentSize.height * 0.66f), Vec2(0,contentSize.height * 0.66f)};
		_patternHider->drawSolidPoly(points, 4, Color4F(Style::Color::darkIndigo));
	}
	else
	{
		Vec2 points[3] = {Vec2(contentSize.width, 0), Vec2(contentSize.width, contentSize.height), Vec2(0,contentSize.height)};
		_patternHider->drawSolidPoly(points, 3, Color4F(Style::Color::darkIndigo));
	}
	
	_gradient->setEndOpacity(isPortrait ? 166 : 245);
	_gradient->setVector(isPortrait ? Vec2(0.0f, -1.0f) : Vec2(0.0f, 1.0f));
	_gradient->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE);
	_gradient->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_TOP);
	_gradient->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(Vec2(contentSize).distance(Vec2(0,0)),Vec2(contentSize).distance(Vec2(0,0)) / 2.0f));
	_gradient->setRotation(CC_RADIANS_TO_DEGREES(isPortrait ? 0 : Vec2(contentSize).getAngle()));
	
	_titleText->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.85f) : Vec2(0.25f,0.5f));
	_titleText->setTextAreaSize(Size(contentSize.width * (isPortrait ? 0.7f : 0.35f),contentSize.height * 0.28f));
	
	for(auto page : _pages)
	{
		if(page.first == kTermsPageKey)
		{
			page.second->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
			page.second->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE);
			page.second->setSizePercent(isPortrait ? Vec2(1.0f,0.71f) : Vec2(0.5f, 1.0f));
		}
		else
		{
			page.second->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
			page.second->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
			page.second->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
		}
	}
}

void SignupScene::changeToPage(const std::string& pageKey)
{
	if(_activePage)
	{
		_activePage->setVisible(false);
		SignupPage* signupPage = dynamic_cast<SignupPage*>(_activePage);
		if(signupPage)
		{
			signupPage->repositionForKeyboardHeight(0, info.duration);
		}
	}
	_activePage = _page.at(pageKey);
	_activePage->setVisible(true);
}

// - IMEDelegate
void SignupScene::keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
	int keyboardHeight = info.end.size.height - Director::getInstance()->getVisibleOrigin().y;
	SignupPage* signupPage = dynamic_cast<SignupPage*>(_activePage);
	if(signupPage)
	{
		signupPage->repositionForKeyboardHeight(keyboardHeight, info.duration);
	}
}
void SignupScene::keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
	SignupPage* signupPage = dynamic_cast<SignupPage*>(_activePage);
	if(signupPage)
	{
		signupPage->repositionForKeyboardHeight(0, info.duration);
	}
}

NS_AZOOMEE_END
