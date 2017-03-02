#include "OnboardingScene.h"
#include "AnalyticsSingleton.h"
#include "ElectricDreamsDecoration.h"
#include "StringMgr.h"
#include "ElectricDreamsTextStyles.h"
#include "TextInputChecker.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "BackEndCaller.h"
#include "AudioMixer.h"

Scene* OnboardingScene::createScene(long errorCode)
{
    auto scene = Scene::create();
    auto layer = OnboardingScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = errorCode;
    
    return scene;
}

bool OnboardingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    AnalyticsSingleton::getInstance()->OnboardingStartEvent();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualElementsToScene();
    addLabelToScene();
    addTextboxScene();
    addButtonsScene();
    
    return true;
}

void OnboardingScene::onEnterTransitionDidFinish()
{
    currentScreen = emailOnboardinScreen;
    
    if(_errorCode !=0)
    {
        MessageBox::createWith(_errorCode, emailTextInput, this);
    }
    else
        emailTextInput->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------
void OnboardingScene::addVisualElementsToScene()
{
    addGlowToScreen(this, 1);
    addSideWiresToScreen(this, 0, 2);
}

void OnboardingScene::addLabelToScene()
{
    title = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL));
    title->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.9);
    this->addChild(title);
    
    subTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL));
    subTitle->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.82);
    subTitle->setVisible(false);
    this->addChild(subTitle);
}

void OnboardingScene::addTextboxScene()
{
    emailTextInput = TextInputLayer::createWithSize(Size(1500,131), INPUT_IS_EMAIL);
    emailTextInput->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.73));
    emailTextInput->setDelegate(this);
    this->addChild(emailTextInput);
    
    passwordTextInput = TextInputLayer::createWithSize(Size(1500,131), INPUT_IS_PASSWORD);
    passwordTextInput->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.73));
    passwordTextInput->setDelegate(this);
    passwordTextInput->setEditboxVisibility(false);
    this->addChild(passwordTextInput);
    
    pinTextInput = TextInputLayer::createWithSize(Size(400,131), INPUT_IS_PIN);
    pinTextInput->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.73));
    pinTextInput->setDelegate(this);
    pinTextInput->setEditboxVisibility(false);
    this->addChild(pinTextInput);
}

void OnboardingScene::addButtonsScene()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(origin.x +backButton->getContentSize().width*.7, origin.y + visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    this->addChild(backButton);
    
    nextButton = ElectricDreamsButton::createNextButton();
    nextButton->setCenterPosition(Vec2(origin.x + visibleSize.width -nextButton->getContentSize().width*.7, origin.y+ visibleSize.height - nextButton->getContentSize().height*.7));
    nextButton->setDelegate(this);
    nextButton->setVisible(false);
    this->addChild(nextButton);
}

//------------CHANGE SCREEN VISUALS ON BUTTON PRESS----------------------
void OnboardingScene::clearElementsOnScreen()
{
    emailTextInput->setEditboxVisibility(false);
    passwordTextInput->setEditboxVisibility(false);
    pinTextInput->setEditboxVisibility(false);
    subTitle->setVisible(false);
    nextButton->setVisible(false);
}

void OnboardingScene::changeElementsToPasswordScreen()
{
    clearElementsOnScreen();
    title->setString(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PASSWORD_LABEL));
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(emailTextInput->getText());
    passwordTextInput->setEditboxVisibility(true);
    currentScreen = passwordOnboardingScreen;
    pinTextInput->setText("");
    passwordTextInput->focusAndShowKeyboard();
}

void OnboardingScene::changeElementsToEmailScreen()
{
    clearElementsOnScreen();
    title->setString(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL));
    passwordTextInput->setText("");
    emailTextInput->setEditboxVisibility(true);
    currentScreen = emailOnboardinScreen;
    nextButton->setVisible(isValidEmailAddress(emailTextInput->getText().c_str()));
    emailTextInput->focusAndShowKeyboard();
}

void OnboardingScene::changeElementsToPinScreen()
{
    clearElementsOnScreen();
    title->setString(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL));
    subTitle->setVisible(true);
    pinTextInput->setEditboxVisibility(true);
    currentScreen = pinOnboardingScreen;
    pinTextInput->focusAndShowKeyboard();
}

void OnboardingScene::backButtonPressed()
{
    if(currentScreen == emailOnboardinScreen)
    {
        HQHistoryManager::getInstance()->emptyHistory();
        auto baseScene = BaseScene::createScene();
        Director::getInstance()->replaceScene(baseScene);
    }
    else if(currentScreen == passwordOnboardingScreen)
        changeElementsToEmailScreen();
    else if(currentScreen == pinOnboardingScreen)
        changeElementsToPasswordScreen();
}

void OnboardingScene::nextButtonPressed()
{
    if(currentScreen == emailOnboardinScreen)
    {
        AnalyticsSingleton::getInstance()->OnboardingEmailSubmittedEvent(emailTextInput->getText());
        changeElementsToPasswordScreen();
    }
    else if(currentScreen == passwordOnboardingScreen)
    {
        AnalyticsSingleton::getInstance()->OnboardingPasswordSubmittedEvent();
        changeElementsToPinScreen();
    }
    else if(currentScreen == pinOnboardingScreen)
        signUp();
}

//------------PRIVATE OTHER FUNCTIONS------------

void OnboardingScene::signUp()
{
    AnalyticsSingleton::getInstance()->OnboardingPinSubmittedEvent();
    
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->registerParent(emailTextInput->getText(), passwordTextInput->getText(), pinTextInput->getText());
}

//----------------------- Delegate Functions ----------------------------

void OnboardingScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    nextButton->setVisible(isValid);
}

void OnboardingScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == nextButton)
        nextButtonPressed();
    else if(button == backButton)
        backButtonPressed();
}

void OnboardingScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(currentScreen == emailOnboardinScreen)
        emailTextInput->focusAndShowKeyboard();
    else if(currentScreen == passwordOnboardingScreen)
        passwordTextInput->focusAndShowKeyboard();
    else if(currentScreen == pinOnboardingScreen)
        pinTextInput->focusAndShowKeyboard();
}
