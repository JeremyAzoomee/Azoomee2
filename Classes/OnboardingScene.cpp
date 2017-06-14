#include "OnboardingScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "SceneManagerScene.h"
#include "FlowDataSingleton.h"

using namespace Azoomee;


Scene* OnboardingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OnboardingScene::create();
    scene->addChild(layer);
    
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
    
    return true;
}

void OnboardingScene::onEnter()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addMainTitleToScene();
    addTextboxScene();
    addLabelsToScene();
    addButtonsScene();
    
    Node::onEnter();
}

void OnboardingScene::onEnterTransitionDidFinish()
{
    if(FlowDataSingleton::getInstance()->hasError())
    {
        emailTextInput->setEditboxVisibility(false);
        passwordTextInput->setEditboxVisibility(false);
        pinTextInput->setEditboxVisibility(false);
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), this);
    }
    else
        emailTextInput->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------

void OnboardingScene::addMainTitleToScene()
{
    mainTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_TITLE_LABEL));
    this->addChild(mainTitle);
}

void OnboardingScene::addTextboxScene()
{
    float textInputWidth = visibleSize.width*.80;
    
    emailTextInput = TextInputLayer::createWithSize(Size(textInputWidth,197), INPUT_IS_EMAIL);
    emailTextInput->setPositionY(mainTitle->getPositionY()-emailTextInput->getContentSize().height*2.1);
    emailTextInput->setDelegate(this);
    emailTextInput->setText(FlowDataSingleton::getInstance()->getUserName());
    this->addChild(emailTextInput);
    
    passwordTextInput = TextInputLayer::createWithSize(Size(textInputWidth,197), INPUT_IS_PASSWORD);
    passwordTextInput->setPositionY(emailTextInput->getPositionY() -passwordTextInput->getContentSize().height*1.9 );
    passwordTextInput->setDelegate(this);
    this->addChild(passwordTextInput);
    
    pinTextInput = TextInputLayer::createWithSize(Size(600,197), INPUT_IS_PIN);
    pinTextInput->setPositionY(passwordTextInput->getPositionY() -pinTextInput->getContentSize().height*1.9 );
    pinTextInput->setDelegate(this);
    this->addChild(pinTextInput);
}

void OnboardingScene::addLabelsToScene()
{
    Label* emailTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL));
    emailTitle->setPositionY(emailTextInput->getPositionY()+(emailTextInput->getContentSize().height) + (emailTitle->getContentSize().height*.6));
    this->addChild(emailTitle);
    
    Label* passwordTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PASSWORD_LABEL));
    passwordTitle->setPositionY(passwordTextInput->getPositionY()+(passwordTextInput->getContentSize().height) + (passwordTitle->getContentSize().height*.6));
    this->addChild(passwordTitle);
    
    Label* pinTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL));
    pinTitle->setPositionY(pinTextInput->getPositionY()+(pinTextInput->getContentSize().height) + (pinTitle->getContentSize().height*.6));
    this->addChild(pinTitle);
    
    Label* pinSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL));
    pinSubTitle->setPositionY(pinTextInput->getPositionY() - (pinSubTitle->getContentSize().height*.2));
    pinSubTitle->setLineSpacing(20);
    this->addChild(pinSubTitle);
}

void OnboardingScene::addButtonsScene()
{
    signupButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE));
    signupButton->setCenterPosition(Vec2(visibleSize.width*.75+origin.x, pinTextInput->getPositionY()-signupButton->getContentSize().height*1.5));
    signupButton->setDelegate(this);
    signupButton->setMixPanelButtonName("signupSceneContinueButton");
    signupButton->setVisible(false);
    
    auto placeHolder = ElectricDreamsButton::createPlaceHolderButton(signupButton->getContentSize().width);
    placeHolder->setCenterPosition(signupButton->getCenterPosition());
    this->addChild(placeHolder);
    this->addChild(signupButton);
    
    cancelButton = ElectricDreamsButton::createOutlineButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL));
    cancelButton->setCenterPosition(Vec2(visibleSize.width*.25+origin.x, signupButton->getCenterPosition().y));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("signupSceneCancelButton");
    this->addChild(cancelButton);
}

//------------PRIVATE OTHER FUNCTIONS------------

void OnboardingScene::signUp()
{
    ModalMessages::getInstance()->startLoading();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(emailTextInput->getText());
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->registerParent(emailTextInput->getText(), passwordTextInput->getText(), pinTextInput->getText());
}

//----------------------- Delegate Functions ----------------------------

void OnboardingScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(emailTextInput->inputIsValid() && passwordTextInput->inputIsValid() && pinTextInput->inputIsValid())
        signupButton->setVisible(true);
    else
        signupButton->setVisible(false);
}

void OnboardingScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer == emailTextInput)
    {
        passwordTextInput->focusAndShowKeyboard();
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(emailTextInput->getText());
    }
    else if(inputLayer == passwordTextInput)
        pinTextInput->focusAndShowKeyboard();
    else if(inputLayer == pinTextInput)
        if(emailTextInput->inputIsValid() && passwordTextInput->inputIsValid() && pinTextInput->inputIsValid())
            signUp();
}

void OnboardingScene::buttonPressed(ElectricDreamsButton* button)
{
    //setOrientationToLandscape();
    
    if(button == signupButton)
        signUp();
    else if(button == cancelButton)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(BaseWithNoHistory));
    }
}

void OnboardingScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(buttonTitle == "Log in")
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
    }
    else
    {
        emailTextInput->setEditboxVisibility(true);
        passwordTextInput->setEditboxVisibility(true);
        pinTextInput->setEditboxVisibility(true);
        emailTextInput->focusAndShowKeyboard();
    }
}

