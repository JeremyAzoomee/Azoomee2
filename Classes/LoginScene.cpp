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
#include "FlowDataSingleton.h"
#include "SceneManagerScene.h"
#include "ApplePaymentSingleton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN


Scene* LoginScene::createScene()
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
    cocos2d::log("Cache folder: %s", FileUtils::getInstance()->getWritablePath().c_str());
    cocos2d::log("DEVICEDATA: %s", ApplePaymentSingleton::getInstance()->getIosDeviceData());
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    getUserDefaults();
    addTextboxScene();
    addSideWiresToScreen(this, 0, 2);
    addLabelToScene();
    addButtonsScene();
    
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

void LoginScene::addLabelToScene()
{
    auto versionTitle = createLabelAppVerison(ConfigStorage::getInstance()->getVersionNumberToDisplay());
    this->addChild(versionTitle);

    title = createLabelHeader(StringMgr::getInstance()->getStringForKey(LOGINSCENE_EMAIL_LABEL));
    this->addChild(title);
}

void LoginScene::addTextboxScene()
{
    passwordTextInput = TextInputLayer::createWithSize(Size(1500,197), INPUT_IS_PASSWORD);
    passwordTextInput->setDelegate(this);
    passwordTextInput->setEditboxVisibility(false);
    this->addChild(passwordTextInput);
    
    emailTextInput = TextInputLayer::createWithSize(Size(1500,197), INPUT_IS_EMAIL);
    emailTextInput->setDelegate(this);
    emailTextInput->setText(storedUsername);
    this->addChild(emailTextInput);
}

void LoginScene::addButtonsScene()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(origin.x +backButton->getContentSize().width*.7, origin.y + visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    backButton->setMixPanelButtonName("LoginSceneBackButton");
    this->addChild(backButton);
    
    nextButton = ElectricDreamsButton::createNextButton();
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
    CCLOG("NEXT NextButton visible false - change to password");
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
    CCLOG("NEXT NextButton visible change to email %d",isValidEmailAddress(emailTextInput->getText().c_str()));
    nextButton->setVisible(isValidEmailAddress(emailTextInput->getText().c_str()));
    emailTextInput->focusAndShowKeyboard();
}

void LoginScene::backButtonPressed()
{
    if(currentScreen == emailLoginScreen)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(BaseWithNoHistory));
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
    CCLOG("NEXT Check Input TextBox is %d", isValid);
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
    if(messageBoxTitle == StringMgr::getInstance()->getErrorMessageWithCode(ERROR_CODE_INVALID_CREDENTIALS)[ERROR_TITLE] && buttonTitle == "Reset Password")
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

NS_AZOOMEE_END
