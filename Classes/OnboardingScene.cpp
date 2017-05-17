#include "OnboardingScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "OrientationChangeScene.h"

using namespace Azoomee;


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
    
    hasBeenSet = false;
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    AnalyticsSingleton::getInstance()->OnboardingStartEvent();
    
    return true;
}

void OnboardingScene::onEnter()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addTextboxScene();
    addLabelsToScene();
    addButtonsScene();
    
    Node::onEnter();
}

void OnboardingScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        emailTextInput->setVisible(false);
        passwordTextInput->setVisible(false);
        pinTextInput->setVisible(false);
        MessageBox::createWith(_errorCode, this);
    }
    else
        emailTextInput->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------
void OnboardingScene::addTextboxScene()
{
    float textInputWidth = visibleSize.width*.90;
    
    emailTextInput = TextInputLayer::createWithSize(Size(textInputWidth,197), INPUT_IS_EMAIL);
    emailTextInput->setPositionY(visibleSize.height-emailTextInput->getContentSize().height*1.75);
    emailTextInput->setDelegate(this);
    this->addChild(emailTextInput);
    
    passwordTextInput = TextInputLayer::createWithSize(Size(textInputWidth,197), INPUT_IS_PASSWORD);
    passwordTextInput->setPositionY(emailTextInput->getPositionY() -passwordTextInput->getContentSize().height*2 );
    passwordTextInput->setDelegate(this);
    this->addChild(passwordTextInput);
    
    pinTextInput = TextInputLayer::createWithSize(Size(600,197), INPUT_IS_PIN);
    pinTextInput->setPositionY(passwordTextInput->getPositionY() -pinTextInput->getContentSize().height*2.7 );
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
    
    Label* pinSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL));
    pinSubTitle->setPositionY(pinTextInput->getPositionY()+(pinTextInput->getContentSize().height) + (pinSubTitle->getContentSize().height*1.15));
    this->addChild(pinSubTitle);
    
    Label* pinTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL));
    pinTitle->setPositionY(pinSubTitle->getPositionY() + pinTitle->getContentSize().height/2);
    this->addChild(pinTitle);
}

void OnboardingScene::addButtonsScene()
{
    signupButton = ElectricDreamsButton::createButtonWithText("Signup");
    signupButton->setCenterPosition(Vec2(visibleSize.width*.75+origin.x, pinTextInput->getPositionY()-signupButton->getContentSize().height*1.2));
    signupButton->setDelegate(this);
    signupButton->setMixPanelButtonName("signupSceneSignupButton");
    signupButton->setVisible(false);
    
    auto placeHolder = ElectricDreamsButton::createPlaceHolderButton(signupButton->getContentSize().width);
    placeHolder->setCenterPosition(signupButton->getCenterPosition());
    this->addChild(placeHolder);
    this->addChild(signupButton);
    
    cancelButton = ElectricDreamsButton::createOutlineButtonWithText("Cancel");
    cancelButton->setCenterPosition(Vec2(visibleSize.width*.25+origin.x, signupButton->getCenterPosition().y));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("signupSceneCancelButton");
    this->addChild(cancelButton);
}

//------------PRIVATE OTHER FUNCTIONS------------

void OnboardingScene::signUp()
{
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
        auto orientationChangeScene = OrientationChangeScene::createScene(false, BASE_SCENE, 0);
        Director::getInstance()->replaceScene(orientationChangeScene);
    }
}

void OnboardingScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    emailTextInput->setVisible(true);
    passwordTextInput->setVisible(true);
    pinTextInput->setVisible(true);
    emailTextInput->focusAndShowKeyboard();
}

