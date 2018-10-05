#include "LoginScene.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Strings.h>
#include "BaseScene.h"
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
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Vec2(0,0);
    
    addBackground();
    getUserDefaults();
    addTextboxScene();
    addButtonsScene();
    addLabelToScene();
    
    return true;
}

void LoginScene::onEnterTransitionDidFinish()
{
    _currentScreen = emailLoginScreen;

    OfflineChecker::getInstance()->setDelegate(this);
    
    if(FlowDataSingleton::getInstance()->hasError())
    {
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), this);
        _emailTextInput->setEditboxVisibility(false);
    }
    else
    {
        _emailTextInput->focusAndShowKeyboard();
    }
    
    _nextButton->setVisible(isValidEmailAddress(_storedUsername.c_str()));
}

//----------------- SCENE SETUP ---------------

void LoginScene::getUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    _storedUsername = def->getStringForKey("username", "");
    def->flush();
    
    if(_storedUsername == "")
    {
        _storedUsername = FlowDataSingleton::getInstance()->getUserName();
    }
}

void LoginScene::addBackground()
{
    auto newLayer = LayerColor::create(Color4B::WHITE, _visibleSize.width,  _visibleSize.height);
    newLayer->setPosition(_origin.x, _origin.y);
    newLayer->setName("bgLayer");
    this->addChild(newLayer, -1);
    
    Sprite* topGradient = Sprite::create("res/decoration/topSignupGrad.png");
    topGradient->setAnchorPoint(Vec2(0.0f, 1.0f));
    topGradient->setPosition(0.0f, _origin.y + _visibleSize.height);
    topGradient->setScaleX(_visibleSize.width / topGradient->getContentSize().width);
    newLayer->addChild(topGradient);
}

void LoginScene::addLabelToScene()
{
    _versionLabel = createLabelAppVerison(ConfigStorage::getInstance()->getVersionNumberToDisplay());
    this->addChild(_versionLabel);

    _title = createLabelFlowMainTitle(_("Log in. Please enter your email."));
    _title->setPositionY(_visibleSize.height * 0.9f);
    _title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_title->setWidth(_visibleSize.width * 0.75f);
    this->addChild(_title);
}

void LoginScene::addTextboxScene()
{
    float width = MIN(1500, _visibleSize.width * 0.95f);
    _passwordTextInput = TextInputLayer::createWithSize(Size(width,160), INPUT_IS_PASSWORD);
    _passwordTextInput->setDelegate(this);
    _passwordTextInput->setEditboxVisibility(false);
    _passwordTextInput->setCenterPosition(Vec2(_visibleSize.width / 2.0f, _visibleSize.height * 0.7f));
    this->addChild(_passwordTextInput);
    
    _emailTextInput = TextInputLayer::createWithSize(Size(width,160), INPUT_IS_EMAIL);
    _emailTextInput->setDelegate(this);
    _emailTextInput->setText(_storedUsername);
    _emailTextInput->setCenterPosition(Vec2(_visibleSize.width / 2.0f, _visibleSize.height * 0.7f));
    this->addChild(_emailTextInput);
}

void LoginScene::addButtonsScene()
{
    _backButton = ElectricDreamsButton::createBackButtonGreen();
    _backButton->setCenterPosition(Vec2(_origin.x +_backButton->getContentSize().width*.7, _origin.y + _visibleSize.height - _backButton->getContentSize().height*.7));
    _backButton->setDelegate(this);
    _backButton->setMixPanelButtonName("LoginSceneBackButton");
    this->addChild(_backButton);
    
    _nextButton = ElectricDreamsButton::createNextButtonGreen();
    _nextButton->setCenterPosition(Vec2(_origin.x + _visibleSize.width - _nextButton->getContentSize().width*.7, _origin.y+ _visibleSize.height - _nextButton->getContentSize().height*.7));
    _nextButton->setDelegate(this);
    _nextButton->setMixPanelButtonName("LoginSceneNextButton");
    this->addChild(_nextButton);
}

//------------CHANGE SCREEN VISUALS ON BUTTON PRESS----------------------
void LoginScene::changeElementsToPasswordScreen()
{
    _title->setString(_("Password"));
    _storedUsername = _emailTextInput->getText();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(_storedUsername);
    _emailTextInput->setEditboxVisibility(false);
    _passwordTextInput->setEditboxVisibility(true);
    _nextButton->setVisible(false);
    _currentScreen = passwordLoginScreen;
    _passwordTextInput->focusAndShowKeyboard();
}

void LoginScene::changeElementsToEmailScreen()
{
    _title->setString(_("Log in. Please enter your email."));
    _passwordTextInput->setEditboxVisibility(false);
    _passwordTextInput->setText("");
    _emailTextInput->setEditboxVisibility(true);
    _currentScreen = emailLoginScreen;
    _nextButton->setVisible(isValidEmailAddress(_emailTextInput->getText().c_str()));
    _emailTextInput->focusAndShowKeyboard();
}

void LoginScene::backButtonPressed()
{
    if(_currentScreen == emailLoginScreen)
    {
        BackEndCaller::getInstance()->anonymousDeviceLogin();
    }
    else if(_currentScreen == passwordLoginScreen)
    {
        changeElementsToEmailScreen();
    }
}

void LoginScene::nextButtonPressed()
{
    if(_currentScreen == emailLoginScreen)
    {
        changeElementsToPasswordScreen();
    }
    else if(_currentScreen == passwordLoginScreen)
    {
        FlowDataSingleton::getInstance()->setFlowToLogin();
        OfflineChecker::getInstance()->setDelegate(nullptr);
        login(_storedUsername, _passwordTextInput->getText());
    }
}

//------------PRIVATE OTHER FUNCTIONS------------

void LoginScene::login(std::string username, std::string password)
{
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

//-------------DELEGATE FUNCTIONS-------------------
void LoginScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _nextButton->setVisible(isValid);
}

void LoginScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(_currentScreen == emailLoginScreen && _emailTextInput->inputIsValid())
    {
        nextButtonPressed();
    }
    else if(_currentScreen == passwordLoginScreen && _passwordTextInput->inputIsValid())
    {
        nextButtonPressed();
    }
}

void LoginScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}

void LoginScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

void LoginScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == _nextButton)
    {
        nextButtonPressed();
    }
    else if(button == _backButton)
    {
        backButtonPressed();
    }
}
void LoginScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_INVALID_CREDENTIALS)[ERROR_TITLE] && buttonTitle == _(MessageBox::kResetPassword))
    {
        BackEndCaller::getInstance()->resetPasswordRequest(_emailTextInput->getText());
        Azoomee::MessageBox::createWith(_("Request Sent"), StringUtils::format(_("Instructions for resetting your password have been sent to:\n\n%s").c_str(),_emailTextInput->getText().c_str()), _("OK") , this);
    }
    else
    {
        _emailTextInput->setEditboxVisibility(true);
        _emailTextInput->focusAndShowKeyboard();
    }
}

void LoginScene::connectivityStateChanged(bool online)
{
    if(!online)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
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
    if(!isVisible())
    {
        return;
    }
    
    // Take into account screen cropping
    int keyboardHeight = info.end.size.height - Director::getInstance()->getVisibleOrigin().y;
    
    ConfigStorage::getInstance()->setEstimatedKeyboardHeight(keyboardHeight);
}

void LoginScene::keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
    if(!isVisible())
    {
        return;
    }
    
    // Take into account screen cropping
    int keyboardHeight = info.end.size.height - Director::getInstance()->getVisibleOrigin().y;
    
    ConfigStorage::getInstance()->setEstimatedKeyboardHeight(keyboardHeight);
}

void LoginScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    // If initialised
    if(_emailTextInput)
    {
        this->removeChildByName("bgLayer");
        addBackground();
        
        _versionLabel->setPosition(_visibleSize.width/2, _versionLabel->getContentSize().height);
        _title->setPosition(_visibleSize.width/2, _visibleSize.height * 0.9);
        _title->setWidth(_visibleSize.width * 0.9f);
        _backButton->setCenterPosition(Vec2(_backButton->getContentSize().width*.7, _visibleSize.height - _backButton->getContentSize().height*.7));
        _nextButton->setCenterPosition(Vec2(_visibleSize.width - _nextButton->getContentSize().width*.7, _visibleSize.height - _nextButton->getContentSize().height*.7));
        
        // Horribly, we have to re-create the editboxes. If we don't the native OS view gets stuck offset
        std::string username = _emailTextInput->getText();
        std::string password = _passwordTextInput->getText();
        bool emailInputVisiblility = _emailTextInput->getEditboxVisibility();
        bool passwordInputVisiblility = _passwordTextInput->getEditboxVisibility();
        _emailTextInput->removeFromParent();
        _passwordTextInput->removeFromParent();
        addTextboxScene();
        
        _emailTextInput->setText(username);
        _passwordTextInput->setText(password);
        
        _emailTextInput->setEditboxVisibility(emailInputVisiblility);
        _passwordTextInput->setEditboxVisibility(passwordInputVisiblility);
    }
}

NS_AZOOMEE_END
