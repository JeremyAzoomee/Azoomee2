//
//  SignupScene.cpp
//  Azoomee
//
//  Created by Macauley on 15/07/2019.
//

#include "SignupScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include "SignupEnterEmail.h"
#include "SignupConfirmEmail.h"
#include "SignupEnterPassword.h"
#include "SignupEnterPin.h"
#include "SignupTermsPage.h"
#include "PopupMessageBox.h"
#include "BackEndCaller.h"
#include "SceneManagerScene.h"
#include "PopupMessageBox.h"
#include "LoginLogicHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string SignupScene::kEnterEmailPageKey = "enterEmail";
const std::string SignupScene::kConfirmEmailPageKey = "confirmEmail";
const std::string SignupScene::kEnterPasswordPageKey = "enterPassword";
const std::string SignupScene::kEnterPinPageKey = "enterPin";
const std::string SignupScene::kTermsPageKey = "terms";

const std::map<std::string, cocos2d::Color3B> SignupScene::kPagePatternColours = {
	{kEnterEmailPageKey, Style::Color::macaroniAndCheese},
	{kConfirmEmailPageKey, Style::Color::macaroniAndCheese},
	{kEnterPasswordPageKey, Style::Color::purplyPink},
	{kEnterPinPageKey, Style::Color::greenishCyan},
	{kTermsPageKey, Style::Color::azure}
};

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
	
	_titleText = ui::Text::create(_("Create your family account"), Style::Font::PoppinsBold(), 105);
	_titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_titleText->setPosition(isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f));
	_titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
	_titleText->setTextAreaSize(Size(contentSize.width * (isPortrait ? 0.7f : 0.35f),contentSize.height * 0.28f));
	_titleText->setTextColor(Color4B::WHITE);
	addChild(_titleText);
	
	SignupEnterEmail* enterEmail = SignupEnterEmail::create();
	enterEmail->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	enterEmail->setNormalizedPosition(isPortrait ? Vec2(0.5,0.0) : Vec2::ANCHOR_MIDDLE);
	enterEmail->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	enterEmail->setSizePercent(isPortrait ? Vec2(1.0f,0.72f) : Vec2(0.5f, 0.95f));
	enterEmail->setVisible(false);
	enterEmail->setContinueCallback([this](const std::string& inputString){
		_signupData._email = inputString;
		this->changeToPage(kConfirmEmailPageKey);
	});
	enterEmail->setBackCallback([this](const std::string& inputString){
		_signupData._email = inputString;
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::IAP));
	});
	enterEmail->setName(kEnterEmailPageKey);
	_pages.insert(kEnterEmailPageKey, enterEmail);
	addChild(enterEmail);
	
	SignupConfirmEmail* confirmEmail = SignupConfirmEmail::create();
	confirmEmail->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	confirmEmail->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	confirmEmail->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	confirmEmail->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	confirmEmail->setVisible(false);
	confirmEmail->setContinueCallback([this](const std::string& inputString){
		this->changeToPage(kEnterPasswordPageKey);
	});
	confirmEmail->setBackCallback([this](const std::string& inputString){
		this->changeToPage(kEnterEmailPageKey);
	});
	confirmEmail->setName(kConfirmEmailPageKey);
	_pages.insert(kConfirmEmailPageKey, confirmEmail);
	addChild(confirmEmail);
	
	SignupEnterPassword* enterPassword = SignupEnterPassword::create();
	enterPassword->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	enterPassword->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	enterPassword->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	enterPassword->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	enterPassword->setVisible(false);
	enterPassword->setContinueCallback([this](const std::string& inputString){
		_signupData._password = inputString;
		this->changeToPage(kEnterPinPageKey);
	});
	enterPassword->setBackCallback([this](const std::string& inputString){
		_signupData._password = inputString;
		this->changeToPage(kConfirmEmailPageKey);
	});
	enterPassword->setName(kEnterPasswordPageKey);
	_pages.insert(kEnterPasswordPageKey, enterPassword);
	addChild(enterPassword);
	
	SignupEnterPin* enterPin = SignupEnterPin::create();
	enterPin->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	enterPin->setNormalizedPosition(isPortrait ? Vec2(0.5,0.025) : Vec2::ANCHOR_MIDDLE);
	enterPin->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	enterPin->setSizePercent(isPortrait ? Vec2(0.95f,0.67f) : Vec2(0.45f, 0.9f));
	enterPin->setVisible(false);
	enterPin->setContinueCallback([this](const std::string& inputString){
		_signupData._pin = inputString;
		this->changeToPage(kTermsPageKey);
	});
	enterPin->setName(kEnterPinPageKey);
	enterPin->setBackCallback([this](const std::string& inputString){
		_signupData._pin = inputString;
		this->changeToPage(kEnterPasswordPageKey);
	});
	_pages.insert(kEnterPinPageKey, enterPin);
	addChild(enterPin);
	
	SignupTermsPage* termsPage = SignupTermsPage::create();
	termsPage->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	termsPage->setNormalizedPosition(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE);
	termsPage->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	termsPage->setSizePercent(isPortrait ? Vec2(1.0f,0.71f) : Vec2(0.5f, 1.0f));
	termsPage->setVisible(false);
	termsPage->setSubmitCallback([this](bool over18, bool acceptTerms, bool acceptMarketing){
		if(over18 && acceptTerms)
		{
			_signupData._acceptMarketing = acceptMarketing;
			ModalMessages::getInstance()->startLoading();
            HttpRequestCreator* request = API::RegisterParentRequest(ParentManager::getInstance()->getLoggedInParentId(), _signupData._email, _signupData._password, _signupData._pin,ConfigStorage::kSignupPlatformSource, ConfigStorage::getInstance()->getDeviceInformation(), _signupData._acceptMarketing ? "true" : "false", this);
			request->execute();
		}
		else
		{
			//show error
			PopupMessageBox* messageBox = PopupMessageBox::create();
			messageBox->setTitle(_("Setup unsuccessful"));
			messageBox->setBody(_("To continue using Azoomee, you must confirm that you are over 18, and that you agree to our Policies."));
			messageBox->setButtonText(_("Back"));
			messageBox->setButtonColour(Style::Color::darkIndigo);
			messageBox->setPatternColour(Style::Color::azure);
			messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
				pSender->removeFromParent();
			});
			this->addChild(messageBox, 1);
		}
	});
	termsPage->setBackCallback([this](bool over18, bool acceptTerms, bool acceptMarketing){
		this->changeToPage(kEnterPinPageKey);
	});
	termsPage->setName(kTermsPageKey);
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
		if(_activePage->getName() == kTermsPageKey)
		{
			Vec2 points[4] = {Vec2(contentSize.width / 2,-1), Vec2(contentSize.width, -1), Vec2(contentSize.width, contentSize.height), Vec2(contentSize.width / 2,contentSize.height)};
			_patternHider->drawSolidPoly(points, 4, Color4F(Style::Color::darkIndigo));
		}
		else
		{
			Vec2 points[3] = {Vec2(contentSize.width, 0), Vec2(contentSize.width, contentSize.height), Vec2(0,contentSize.height)};
			_patternHider->drawSolidPoly(points, 3, Color4F(Style::Color::darkIndigo));
		}
	}
	
	if(_activePage->getName() == kTermsPageKey)
	{
		_gradient->setEndOpacity(isPortrait ? 166 : 255);
		_gradient->setVector(Vec2(0.0f, -1.0f));
		_gradient->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE_BOTTOM);
		_gradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		_gradient->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(contentSize.width, contentSize.height));
		_gradient->setRotation(0);
	}
	else
	{
		_gradient->setEndOpacity(isPortrait ? 166 : 245);
		_gradient->setVector(isPortrait ? Vec2(0.0f, -1.0f) : Vec2(0.0f, 1.0f));
		_gradient->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE);
		_gradient->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_TOP);
		_gradient->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(Vec2(contentSize).distance(Vec2(0,0)),Vec2(contentSize).distance(Vec2(0,0)) / 2.0f));
		_gradient->setRotation(CC_RADIANS_TO_DEGREES(isPortrait ? 0 : Vec2(contentSize).getAngle()));
	}
	_titleText->setPosition(isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f));
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
			page.second->setPosition(isPortrait ? Vec2(contentSize.width * 0.5,0.0) : Vec2(contentSize.width * 0.5f, contentSize.height * 0.5f));
			page.second->setSizePercent(isPortrait ? Vec2(1.0f,0.72f) : Vec2(0.5f, 0.95f));
		}
	}
	
	PopupMessageBox* msgBox = dynamic_cast<PopupMessageBox*>(getChildByName(PopupMessageBox::kPopupMessageBoxName));
	if(msgBox)
	{
		msgBox->onSizeChanged();
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
			signupPage->repositionForKeyboardHeight(0, 0.1f);
		}
	}
	_activePage = _pages.at(pageKey);
	_activePage->setVisible(true);
	_bgPattern->setColor(kPagePatternColours.at(pageKey));
	_titleText->setString(pageKey == kTermsPageKey ? _("Safeguarding your privacy") : _("Create your family account"));
	if(pageKey == kConfirmEmailPageKey)
	{
		SignupConfirmEmail* confirmEmail = dynamic_cast<SignupConfirmEmail*>(_activePage);
		if(confirmEmail)
		{
			confirmEmail->setEmail(_signupData._email);
		}
	}
	else if(pageKey == kEnterPasswordPageKey || pageKey == kEnterPinPageKey)
	{
		SignupPage* page = dynamic_cast<SignupPage*>(_activePage);
		if(page)
		{
			page->clearInputText();
		}
	}
    else if(pageKey == kTermsPageKey)
    {
        SignupTermsPage* termsPage = dynamic_cast<SignupTermsPage*>(_activePage);
        if(termsPage)
        {
            termsPage->setTermsAccepted(false);
            termsPage->setOver18Selected(false);
            termsPage->setMarketingAccepted(false);
        }
    }
	const Size& contentSize = getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	if(_activePage->getName() == kTermsPageKey)
	{
		_gradient->setEndOpacity(isPortrait ? 166 : 255);
		_gradient->setVector(Vec2(0.0f, -1.0f));
		_gradient->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE_BOTTOM);
		_gradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		_gradient->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(contentSize.width, contentSize.height));
		_gradient->setRotation(0);
		if(!isPortrait)
		{
			_patternHider->clear();
			Vec2 points[4] = {Vec2(contentSize.width / 2,-1), Vec2(contentSize.width, -1), Vec2(contentSize.width, contentSize.height), Vec2(contentSize.width / 2,contentSize.height)};
			_patternHider->drawSolidPoly(points, 4, Color4F(Style::Color::darkIndigo));
		}
	}
	else
	{
		_gradient->setEndOpacity(isPortrait ? 166 : 245);
		_gradient->setVector(isPortrait ? Vec2(0.0f, -1.0f) : Vec2(0.0f, 1.0f));
		_gradient->setNormalizedPosition(isPortrait ? Vec2(0.5f,0.66f) : Vec2::ANCHOR_MIDDLE);
		_gradient->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_TOP);
		_gradient->setContentSize(isPortrait ? Size(contentSize.width, contentSize.height * 0.34f) : Size(Vec2(contentSize).distance(Vec2(0,0)),Vec2(contentSize).distance(Vec2(0,0)) / 2.0f));
		_gradient->setRotation(CC_RADIANS_TO_DEGREES(isPortrait ? 0 : Vec2(contentSize).getAngle()));
		
		if(!isPortrait)
		{
			_patternHider->clear();
			Vec2 points[3] = {Vec2(contentSize.width, 0), Vec2(contentSize.width, contentSize.height), Vec2(0,contentSize.height)};
			_patternHider->drawSolidPoly(points, 3, Color4F(Style::Color::darkIndigo));
		}
	}
}

// - IMEDelegate
void SignupScene::keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
	const Size& contentSize = getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	const Vec2& targetPos = isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f);
	
	int keyboardHeight = info.end.size.height - Director::getInstance()->getVisibleOrigin().y;
	if((targetPos.y - (_titleText->getContentSize().height * 0.5f)) < keyboardHeight)
	{
		float offset = keyboardHeight - (targetPos.y - (_titleText->getContentSize().height * 0.5f));
		_titleText->runAction(MoveTo::create(0.5f, targetPos + Vec2(0,offset)));
	}
	
	SignupPage* signupPage = dynamic_cast<SignupPage*>(_activePage);
	if(signupPage)
	{
		float moveDist = signupPage->getMoveDistanceForKeyboardHeight(keyboardHeight);
		if(isPortrait)
		{
			signupPage->runAction(MoveTo::create(0.5f, Vec2(contentSize.width / 2.0f,moveDist)));
		}
		signupPage->repositionForKeyboardHeight(keyboardHeight - moveDist, 0.5f);
	}
}
void SignupScene::keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
	const Size& contentSize = getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	const Vec2& targetPos = isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f);
	_titleText->runAction(MoveTo::create(0.5f, targetPos));
	
	SignupPage* signupPage = dynamic_cast<SignupPage*>(_activePage);
	if(signupPage)
	{
		signupPage->runAction(MoveTo::create(0.5f, isPortrait ? Vec2(contentSize.width * 0.5,0.0) : Vec2(contentSize.width * 0.5f, contentSize.height * 0.5f)));
		signupPage->repositionForKeyboardHeight(0, 0.5f);
	}
}

//Delegate functions
void SignupScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagRegisterParent)
	{
		ModalMessages::getInstance()->stopLoading();
		UserDefault* userDefault = UserDefault::getInstance();
		userDefault->setBoolForKey(ConfigStorage::kAnonOnboardingCompleteKey, false); //registered account is upgrade of local anon account, if logging in as anon again, will need to complete onboarding again for fresh anon account
		userDefault->setStringForKey(ConfigStorage::kAnonEmailKey, "");
		ConfigStorage::getInstance()->setFirstSlideShowSeen();
		AnalyticsSingleton::getInstance()->OnboardingAccountCreatedEvent();
		
		PopupMessageBox* messageBox = PopupMessageBox::create();
		messageBox->setTitle(_("Setup Complete"));
		messageBox->setBody(_("Welcome to the Azoomee family! Your account is now active"));
		messageBox->setButtonText(_("Let's go!"));
		messageBox->setButtonColour(Style::Color::strongPink);
		messageBox->setPatternColour(Style::Color::strongPink);
		messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
			pSender->removeFromParent();
			BackEndCaller::getInstance()->login(_signupData._email, _signupData._password);
		});
		this->addChild(messageBox, 1);
	}
}
void SignupScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
	
	const auto& errorMessageText = StringMgr::getInstance()->getErrorMessageWithCode(errorCode);
	
    if(errorCode == ERROR_CODE_ALREADY_REGISTERED)
    {
        PopupMessageBox* messageBox = PopupMessageBox::create();
        messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
        messageBox->setBody(errorMessageText.at(ERROR_BODY));
        messageBox->setPatternColour(Style::Color::azure);
        
        messageBox->setButtonText(_("Log in"));
        messageBox->setButtonColour(Style::Color::strongPink);
        messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
            pSender->removeFromParent();
            LoginLogicHandler::getInstance()->setLoginOrigin(LoginOrigin::SIGNUP);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
        });
        
        messageBox->setSecondButtonText(_("Back"));
        messageBox->setSecondButtonColour(Style::Color::darkIndigo);
        messageBox->setSecondButtonPressedCallback([this](PopupMessageBox* pSender){
            pSender->removeFromParent();
            this->changeToPage(kEnterEmailPageKey);
        });
        this->addChild(messageBox, 1);
    }
    else
    {
        PopupMessageBox* messageBox = PopupMessageBox::create();
        messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
        messageBox->setBody(errorMessageText.at(ERROR_BODY));
        messageBox->setButtonText(_("Back"));
        messageBox->setButtonColour(Style::Color::darkIndigo);
        messageBox->setPatternColour(Style::Color::azure);
        messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
            pSender->removeFromParent();
        });
        this->addChild(messageBox, 1);
    }
}

NS_AZOOMEE_END
