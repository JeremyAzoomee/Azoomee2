#include "LoginScene.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Strings.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include "OfflineHubScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Utils/PushNotificationsHandler.h>
#include "FlowDataSingleton.h"
#include "SceneManagerScene.h"
#include "ChatNotificationsSingleton.h"
#include "ContentHistoryManager.h"
#include "PopupMessageBox2Buttons.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool LoginScene::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    ChatNotificationsSingleton::getInstance()->stopNotificationsUpdate();
    AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
    
    ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    
    PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel("NA");
    AudioMixer::getInstance()->stopBackgroundMusic();
	
    getUserDefaults();
	
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
	
	_titleText = DynamicText::create(_("Log in"), Style::Font::PoppinsBold(), 105);
	_titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_titleText->setPosition(isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f));
	_titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
	_titleText->setTextAreaSize(Size(contentSize.width * (isPortrait ? 0.7f : 0.35f),contentSize.height * 0.28f));
	_titleText->setTextColor(Color4B::WHITE);
	addChild(_titleText);
	
	_loginEntryForm = LoginEntry::create();
	_loginEntryForm->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	_loginEntryForm->setNormalizedPosition(isPortrait ? Vec2(0.5,0.0) : Vec2::ANCHOR_MIDDLE);
	_loginEntryForm->setSizeType(cocos2d::ui::Layout::SizeType::PERCENT);
	_loginEntryForm->setSizePercent(isPortrait ? Vec2(1.0f,0.72f) : Vec2(0.5f, 0.95f));
	_loginEntryForm->setEmail(_storedUsername);
	_loginEntryForm->setState(LoginEntryState::EMAIL);
	_loginEntryForm->setContinueButtonCallback([this](const LoginEntryState& state){
		switch (state) {
			case LoginEntryState::EMAIL:
			{
				_loginEntryForm->setState(LoginEntryState::PASSWORD);
				_bgPattern->setColor(Style::Color::purplyPink);
				break;
			}
			case LoginEntryState::PASSWORD:
			{
				break;
			}
		}
	});
	_loginEntryForm->setBackButtonCallback([this](const LoginEntryState& state){
		switch (state) {
			case LoginEntryState::EMAIL:
			{
				BackEndCaller::getInstance()->anonymousDeviceLogin();
				break;
			}
			case LoginEntryState::PASSWORD:
			{
				_loginEntryForm->setState(LoginEntryState::EMAIL);
				_bgPattern->setColor(Style::Color::macaroniAndCheese);
				break;
			}
		}
	});
	_loginEntryForm->setLoginConfirmCallback([this](const std::string& email,const std::string& password){
		FlowDataSingleton::getInstance()->setFlowToLogin();
		OfflineChecker::getInstance()->setDelegate(nullptr);
		_storedUsername = email;
		login(email, password);
	});
	addChild(_loginEntryForm);
	
    return true;
}

void LoginScene::onEnter()
{
	Super::onEnter();

    OfflineChecker::getInstance()->setDelegate(this);
    
    if(FlowDataSingleton::getInstance()->hasError())
    {
		_loginEntryForm->setVisible(false);
        
        const long errorCode = FlowDataSingleton::getInstance()->getErrorCode();
        const auto& errorMessageText = StringMgr::getInstance()->getErrorMessageWithCode(errorCode);
        
        if(errorCode == ERROR_CODE_INVALID_CREDENTIALS)
        {
            PopupMessageBox2Buttons* messageBox = PopupMessageBox2Buttons::create();
            messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
            messageBox->setBody(errorMessageText.at(ERROR_BODY));
            messageBox->setButtonText(_("Back"));
            messageBox->setButtonColour(Style::Color::darkIndigo);
            messageBox->setPatternColour(Style::Color::azure);
            messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
                pSender->removeFromParent();
                _loginEntryForm->setVisible(true);
            });
            messageBox->setSecondButtonText(_("Reset password"));
            messageBox->setSecondButtonColour(Style::Color::strongPink);
            messageBox->setSecondButtonPressedCallback([this](PopupMessageBox* pSender){
                pSender->removeFromParent();
                BackEndCaller::getInstance()->resetPasswordRequest(_storedUsername);
                
                PopupMessageBox* messageBox = PopupMessageBox::create();
                messageBox->setTitle(_("Reset requested"));
                messageBox->setBody(StringUtils::format((_("Instructions for resetting your password have been sent to:") + "\n\n%s").c_str(), _storedUsername.c_str()));
                messageBox->setButtonText(_("OK"));
                messageBox->setButtonColour(Style::Color::darkIndigo);
                messageBox->setPatternColour(Style::Color::azure);
                messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
                    pSender->removeFromParent();
                    _loginEntryForm->setVisible(true);
                });
                this->addChild(messageBox, 1);
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
                _loginEntryForm->setVisible(true);
            });
            this->addChild(messageBox, 1);
        }
    }
}

//----------------- SCENE SETUP ---------------

void LoginScene::getUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    _storedUsername = def->getStringForKey(ConfigStorage::kStoredUsernameKey, "");
    def->flush();
    
    if(_storedUsername == "")
    {
        _storedUsername = FlowDataSingleton::getInstance()->getUserName();
    }
}

//------------PRIVATE OTHER FUNCTIONS------------

void LoginScene::login(std::string username, std::string password)
{
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

//-------------DELEGATE FUNCTIONS-------------------

void LoginScene::connectivityStateChanged(bool online)
{
    if(!online)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
    }
}

void LoginScene::onExit()
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
    Super::onExit();
}

#pragma mark - IMEDelegate

void LoginScene::keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
	const Size& contentSize = getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	const Vec2& targetPos = isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f);
	
	int keyboardHeight = info.end.size.height - Director::getInstance()->getVisibleOrigin().y;
	ConfigStorage::getInstance()->setEstimatedKeyboardHeight(keyboardHeight);
	if((targetPos.y - (_titleText->getContentSize().height * 0.5f)) < keyboardHeight)
	{
		float offset = keyboardHeight - (targetPos.y - (_titleText->getContentSize().height * 0.5f));
		_titleText->runAction(MoveTo::create(0.5f, targetPos + Vec2(0,offset)));
	}
	

	_loginEntryForm->repositionForKeyboardHeight(keyboardHeight, 0.5f);
}

void LoginScene::keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
	const Size& contentSize = getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	const Vec2& targetPos = isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f);
	_titleText->runAction(MoveTo::create(0.5f, targetPos));
	
	_loginEntryForm->repositionForKeyboardHeight(0, 0.5f);
}

void LoginScene::onSizeChanged()
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
	
	_titleText->setPosition(isPortrait ? Vec2(contentSize.width * 0.5f,contentSize.height * 0.85f) : Vec2(contentSize.width * 0.25f,contentSize.height * 0.5f));
	_titleText->setTextAreaSize(Size(contentSize.width * (isPortrait ? 0.7f : 0.35f),contentSize.height * 0.28f));
	
	_loginEntryForm->setAnchorPoint(isPortrait ? Vec2::ANCHOR_MIDDLE_BOTTOM : Vec2::ANCHOR_MIDDLE_LEFT);
	_loginEntryForm->setNormalizedPosition(isPortrait ? Vec2(0.5,0.0) : Vec2::ANCHOR_MIDDLE);
	_loginEntryForm->setSizePercent(isPortrait ? Vec2(1.0f,0.72f) : Vec2(0.5f, 0.95f));

}

NS_AZOOMEE_END
