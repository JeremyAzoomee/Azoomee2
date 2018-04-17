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


cocos2d::Scene* LoginScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool LoginScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    ChatNotificationsSingleton::getInstance()->stopNotificationsUpdate();
    AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
    
    ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    
    PushNotificationsHandler::getInstance()->setNamedUserIdentifierForPushChannel("NA");
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addBackground();
    getUserDefaults();
    addTextboxScene();
    //addSideWiresToScreen(this, 0, 2);
    addButtonsScene();
    addLabelToScene();
    
    return true;
}

void LoginScene::onEnterTransitionDidFinish()
{
    currentScreen = emailLoginScreen;

    OfflineChecker::getInstance()->setDelegate(this);
    
    if(FlowDataSingleton::getInstance()->hasError())
    {
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), emailTextInput, this);
    }
    else
        emailTextInput->focusAndShowKeyboard();
    
    nextButton->setVisible(isValidEmailAddress(storedUsername.c_str()));
}

//----------------- SCENE SETUP ---------------

void LoginScene::getUserDefaults()
{
    UserDefault* def = UserDefault::getInstance();
    storedUsername = def->getStringForKey("username", "");
    def->flush();
    
    if(storedUsername == "")
        storedUsername = FlowDataSingleton::getInstance()->getUserName();
}

void LoginScene::addBackground()
{
    auto newLayer = LayerColor::create(Color4B::WHITE, visibleSize.width,  visibleSize.height);
    newLayer->setPosition(origin.x, origin.y);
    this->addChild(newLayer);
    
    Sprite* topGradient = Sprite::create("res/decoration/topSignupGrad.png");
    topGradient->setAnchorPoint(Vec2(0.0f, 1.0f));
    topGradient->setPosition(0.0f, origin.y + visibleSize.height);
    topGradient->setScaleX(visibleSize.width / topGradient->getContentSize().width);
    newLayer->addChild(topGradient);
}

void LoginScene::addLabelToScene()
{
    auto versionTitle = createLabelAppVerison(ConfigStorage::getInstance()->getVersionNumberToDisplay());
    this->addChild(versionTitle);

    title = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    title->setPositionY(backButton->getPositionY());
    this->addChild(title);
}

void LoginScene::addTextboxScene()
{
    passwordTextInput = TextInputLayer::createWithSize(Size(1500,160), INPUT_IS_PASSWORD);
    passwordTextInput->setDelegate(this);
    passwordTextInput->setEditboxVisibility(false);
    this->addChild(passwordTextInput);
    
    emailTextInput = TextInputLayer::createWithSize(Size(1500,160), INPUT_IS_EMAIL);
    emailTextInput->setDelegate(this);
    emailTextInput->setText(storedUsername);
    this->addChild(emailTextInput);
}

void LoginScene::addButtonsScene()
{
    backButton = ElectricDreamsButton::createBackButtonGreen();
    backButton->setCenterPosition(Vec2(origin.x +backButton->getContentSize().width*.7, origin.y + visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    backButton->setMixPanelButtonName("LoginSceneBackButton");
    this->addChild(backButton);
    
    nextButton = ElectricDreamsButton::createNextButtonGreen();
    nextButton->setCenterPosition(Vec2(origin.x + visibleSize.width -nextButton->getContentSize().width*.7, origin.y+ visibleSize.height - nextButton->getContentSize().height*.7));
    nextButton->setDelegate(this);
    nextButton->setMixPanelButtonName("LoginSceneNextButton");
    this->addChild(nextButton);
}

//------------CHANGE SCREEN VISUALS ON BUTTON PRESS----------------------
void LoginScene::changeElementsToPasswordScreen()
{
    title->setString(StringMgr::getInstance()->getStringForKey(LOGINSCENE_PASSWORD_LABEL));
    storedUsername = emailTextInput->getText();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(storedUsername);
    emailTextInput->setEditboxVisibility(false);
    passwordTextInput->setEditboxVisibility(true);
    nextButton->setVisible(false);
    currentScreen = passwordLoginScreen;
    passwordTextInput->focusAndShowKeyboard();
}

void LoginScene::changeElementsToEmailScreen()
{
    title->setString(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    passwordTextInput->setEditboxVisibility(false);
    passwordTextInput->setText("");
    emailTextInput->setEditboxVisibility(true);
    currentScreen = emailLoginScreen;
    nextButton->setVisible(isValidEmailAddress(emailTextInput->getText().c_str()));
    emailTextInput->focusAndShowKeyboard();
}

void LoginScene::backButtonPressed()
{
    if(currentScreen == emailLoginScreen)
    {
        BackEndCaller::getInstance()->anonymousDeviceLogin();
    }
    else if(currentScreen == passwordLoginScreen)
        changeElementsToEmailScreen();
}

void LoginScene::nextButtonPressed()
{
    if(currentScreen == emailLoginScreen)
        changeElementsToPasswordScreen();
    else if(currentScreen == passwordLoginScreen)
    {
        FlowDataSingleton::getInstance()->setFlowToLogin();
        OfflineChecker::getInstance()->setDelegate(nullptr);
        login(storedUsername, passwordTextInput->getText());
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
    nextButton->setVisible(isValid);
}

void LoginScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(currentScreen == emailLoginScreen && emailTextInput->inputIsValid())
        nextButtonPressed();
    else if(currentScreen == passwordLoginScreen && passwordTextInput->inputIsValid())
        nextButtonPressed();
}

void LoginScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}

void LoginScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

void LoginScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == nextButton)
        nextButtonPressed();
    else if(button == backButton)
        backButtonPressed();
}
void LoginScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_INVALID_CREDENTIALS)[ERROR_TITLE] && buttonTitle == MessageBox::kResetPassword)
    {
        BackEndCaller::getInstance()->resetPasswordRequest(emailTextInput->getText());
        Azoomee::MessageBox::createWith("Request Sent", StringUtils::format("Instructions for resetting your password have been sent to:\n\n%s",emailTextInput->getText().c_str()), "OK", this);
    }
    else
        emailTextInput->focusAndShowKeyboard();
}

void LoginScene::connectivityStateChanged(bool online)
{
    if(!online)
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
}

void LoginScene::onExit()
{
    OfflineChecker::getInstance()->setDelegate(nullptr);
    Node::onExit();
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

NS_AZOOMEE_END
