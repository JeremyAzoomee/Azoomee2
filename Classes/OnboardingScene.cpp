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
#include <AzoomeeCommon/Data/Urls.h>
#include <AzoomeeCommon/UI/ModalWebview.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN


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
    
    addBackgroundandDecoration();
    addProgressIndicator();
    addMainTitleToScene();
    addTextboxScene();
    addLabelsToScene();
    addButtonsScene();
    addTermsAndConditionsToScene();
    
    Node::onEnter();
}

void OnboardingScene::onEnterTransitionDidFinish()
{
    if(FlowDataSingleton::getInstance()->hasError())
    {
        _emailTextInput->setEditboxVisibility(false);
        passwordTextInput->setEditboxVisibility(false);
        pinTextInput->setEditboxVisibility(false);
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), this);
    }
    else
        _emailTextInput->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------

void OnboardingScene::addBackgroundandDecoration()
{
    Layer* background = createTopGradientAndParticles();
    background->setPosition(origin.x,origin.y);
    this->addChild(background);
}

void OnboardingScene::addProgressIndicator()
{
    _progressIndicatior = Sprite::create("res/decoration/progress1.png");
    _progressIndicatior->setPosition(origin.x+visibleSize.width/2,origin.y+visibleSize.height - _progressIndicatior->getContentSize().height*1.5);
    this->addChild(_progressIndicatior);
}

void OnboardingScene::addMainTitleToScene()
{
    mainTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_TITLE_LABEL));
    mainTitle->setPositionY(_progressIndicatior->getPositionY()-_progressIndicatior->getContentSize().height-mainTitle->getContentSize().height/2);
    this->addChild(mainTitle);
}

void OnboardingScene::addTextboxScene()
{
    float textInputWidth = visibleSize.width*.80;
    
    _emailTextInput = TextInputLayer::createWithSize(Size(textInputWidth,160), INPUT_IS_EMAIL);
    _emailTextInput->setPositionY(mainTitle->getPositionY()-_emailTextInput->getContentSize().height*2.1);
    _emailTextInput->setDelegate(this);
    _emailTextInput->setText(FlowDataSingleton::getInstance()->getUserName());
    this->addChild(_emailTextInput);
    
    passwordTextInput = TextInputLayer::createWithSize(Size(textInputWidth,160), INPUT_IS_NEW_PASSWORD);
    passwordTextInput->setPositionY(_emailTextInput->getPositionY() -passwordTextInput->getContentSize().height*1.9 );
    passwordTextInput->setDelegate(this);
    this->addChild(passwordTextInput);
    
    pinTextInput = TextInputLayer::createWithSize(Size(600,160), INPUT_IS_PIN);
    pinTextInput->setPositionY(passwordTextInput->getPositionY() -pinTextInput->getContentSize().height*1.9 );
    pinTextInput->setDelegate(this);
    this->addChild(pinTextInput);
}

void OnboardingScene::addLabelsToScene()
{
    _emailTitle =  createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL));
    _emailTitle->setPositionY(_emailTextInput->getPositionY()+(_emailTextInput->getContentSize().height) + (_emailTitle->getContentSize().height*.6));
    this->addChild(_emailTitle);
    
    _passwordTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PASSWORD_LABEL));
    _passwordTitle->setPositionY(passwordTextInput->getPositionY()+(passwordTextInput->getContentSize().height) + (_passwordTitle->getContentSize().height*.6));
    this->addChild(_passwordTitle);
    
    _pinTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL));
    _pinTitle->setPositionY(pinTextInput->getPositionY()+(pinTextInput->getContentSize().height) + (_pinTitle->getContentSize().height*.6));
    this->addChild(_pinTitle);
    
    _pinSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL),Style::Color::black);
    _pinSubTitle->setPositionY(pinTextInput->getPositionY() - (_pinSubTitle->getContentSize().height*.2));
    _pinSubTitle->setLineSpacing(20);
    this->addChild(_pinSubTitle);
}

void OnboardingScene::addButtonsScene()
{
    signupButton = ElectricDreamsButton::createGreenButton(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE), visibleSize.width/3);
    signupButton->setCenterPosition(Vec2(visibleSize.width*.75+origin.x, pinTextInput->getPositionY()-signupButton->getContentSize().height*1.1));
    signupButton->setDelegate(this);
    signupButton->setMixPanelButtonName("signupSceneContinueButton");
    signupButton->setVisible(false);
    
    auto placeHolder = ElectricDreamsButton::createPlaceHolderGreenButton(visibleSize.width/3);
    placeHolder->setCenterPosition(signupButton->getCenterPosition());
    this->addChild(placeHolder);
    this->addChild(signupButton);
    
    cancelButton = ElectricDreamsButton::createSecondaryGreenButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL),visibleSize.width/3);
    cancelButton->setCenterPosition(Vec2(visibleSize.width*.25+origin.x, signupButton->getCenterPosition().y));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("signupSceneCancelButton");
    this->addChild(cancelButton);
}

void OnboardingScene::addTermsAndConditionsToScene()
{
    //---------- CREATE AND ADD LABELS AND BUTTONS
    Label* TermsAndConditionsStartLabel =createLabelWith(StringMgr::getInstance()->getStringForKey(T_and_C_Start_Text),Style::Font::Regular, Style::Color::white, 40);
    this->addChild(TermsAndConditionsStartLabel);
    
    Label* andLabel =createLabelWith(StringMgr::getInstance()->getStringForKey(T_and_C_And),Style::Font::Regular, Style::Color::white, 40);
    this->addChild(andLabel);
    
    privacyButton = ElectricDreamsButton::createTextAsButton(StringMgr::getInstance()->getStringForKey(T_and_C_Privacy_Button), 40, true);
    privacyButton->setMixPanelButtonName("signupScenePrivacyPolicyButton");
    privacyButton->setDelegate(this);
    this->addChild(privacyButton);
    
    termsButton = ElectricDreamsButton::createTextAsButton(StringMgr::getInstance()->getStringForKey(T_and_C_Terms_Button), 40, true);
    termsButton->setMixPanelButtonName("signupSceneermsButton");
    termsButton->setDelegate(this);
    this->addChild(termsButton);
    
    //------- CALCULATE AND SET LOCATION OF ITEMS SO THEY ARE CENTERED
    float TermsAndConditionsStartLabelWidth = TermsAndConditionsStartLabel->getContentSize().width;
    float privacyButtonWidth = privacyButton->getContentSize().width;
    float andLabelWidth = andLabel->getContentSize().width;
    float termsButtonWidth = termsButton->getContentSize().width;
    
    float totalWidth = TermsAndConditionsStartLabelWidth + andLabelWidth + privacyButtonWidth + termsButtonWidth;
    float yPosition = signupButton->getPositionY() - (TermsAndConditionsStartLabel->getContentSize().height*1.2);
    
    //NOTE: Buttons are Layers and the anchor point is (0,0)
    TermsAndConditionsStartLabel->setPosition(origin.x+visibleSize.width/2-totalWidth/2+TermsAndConditionsStartLabelWidth/2,yPosition);
    privacyButton->setCenterPosition(Vec2(TermsAndConditionsStartLabel->getPositionX()+TermsAndConditionsStartLabelWidth/2+privacyButtonWidth/2,yPosition));
    andLabel->setPosition(privacyButton->getPositionX() + privacyButtonWidth + andLabelWidth/2,yPosition);
    termsButton->setCenterPosition(Vec2(andLabel->getPositionX()+andLabelWidth/2+termsButtonWidth/2,yPosition));
}

//------------PRIVATE OTHER FUNCTIONS------------

void OnboardingScene::signUp()
{
    ModalMessages::getInstance()->startLoading();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(_emailTextInput->getText());
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->registerParent(_emailTextInput->getText(), passwordTextInput->getText(), pinTextInput->getText());
}

//----------------------- Delegate Functions ----------------------------

void OnboardingScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(_emailTextInput->inputIsValid() && passwordTextInput->inputIsValid() && pinTextInput->inputIsValid())
        signupButton->setVisible(true);
    else
        signupButton->setVisible(false);
}

void OnboardingScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer == _emailTextInput)
    {
        passwordTextInput->focusAndShowKeyboard();
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(_emailTextInput->getText());
        if(!_emailTextInput->inputIsValid())
           _emailTextInput->setEditboxHasError();
    }
    else if(inputLayer == passwordTextInput)
        pinTextInput->focusAndShowKeyboard();
    else if(inputLayer == pinTextInput)
        if(_emailTextInput->inputIsValid() && passwordTextInput->inputIsValid() && pinTextInput->inputIsValid())
            signUp();
}

void OnboardingScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    inputLayer->setEditboxHasError(false);
}

void OnboardingScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    if(!_emailTextInput->inputIsValid() && inputLayer == _emailTextInput)
        _emailTextInput->setEditboxHasError();
}

void OnboardingScene::buttonPressed(ElectricDreamsButton* button)
{
    //setOrientationToLandscape();
    
    if(button == signupButton)
        signUp();
    else if(button == cancelButton)
        BackEndCaller::getInstance()->anonymousDeviceLogin();
    else if(button == privacyButton)
        ModalWebview::createWithURL(Url::PrivacyPolicy);
    else if(button == termsButton)
        ModalWebview::createWithURL(Url::TermsOfUse);
}

void OnboardingScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(buttonTitle == MessageBox::kLogin)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(Login));
    }
    else
    {
        _emailTextInput->setEditboxVisibility(true);
        passwordTextInput->setEditboxVisibility(true);
        pinTextInput->setEditboxVisibility(true);
        _emailTextInput->focusAndShowKeyboard();
    }
}

NS_AZOOMEE_END
