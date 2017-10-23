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
    setNewLayout();
    
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
    passwordTextInput->setDelegate(this);
    this->addChild(passwordTextInput);
    
    pinTextInput = TextInputLayer::createWithSize(Size(600,160), INPUT_IS_PIN);
    pinTextInput->setDelegate(this);
    this->addChild(pinTextInput);
}

void OnboardingScene::addLabelsToScene()
{
    _emailTitle =  createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL));
    _emailTitle->setPositionY(_emailTextInput->getPositionY()+(_emailTextInput->getContentSize().height) + (_emailTitle->getContentSize().height*.6));
    this->addChild(_emailTitle);
    
    _emailError = createLabelBodyCentred("Email format is incorrect.",Style::Color::watermelon);
    _emailError->setLineSpacing(20);
    _emailError->setVisible(false);
    this->addChild(_emailError);
    
    _passwordTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PASSWORD_LABEL));
    this->addChild(_passwordTitle);
    
    _passwordError = createLabelBodyCentred("Password requires 6 charactors with no spaces.",Style::Color::watermelon);
    _passwordError->setLineSpacing(20);
    _passwordError->setVisible(false);
    this->addChild(_passwordError);
    
    _pinTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL));
    this->addChild(_pinTitle);
    
    _pinError = createLabelBodyCentred("Pin requires 4 numbers.",Style::Color::watermelon);
    _pinError->setLineSpacing(20);
    _pinError->setVisible(false);
    this->addChild(_pinError);
    
    _pinSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL),Style::Color::black);
    _pinSubTitle->setLineSpacing(20);
    this->addChild(_pinSubTitle);
}

void OnboardingScene::addButtonsScene()
{
    signupButton = ElectricDreamsButton::createGreenButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE), visibleSize.width/3);
    signupButton->setMixPanelButtonName("signupSceneContinueButton");
    signupButton->setOpacity(80);
    this->addChild(signupButton);
    
    cancelButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL), 75, true, Color3B::BLACK);
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("signupSceneCancelButton");
    this->addChild(cancelButton);
}

void OnboardingScene::addTermsAndConditionsToScene()
{
    //---------- CREATE AND ADD LABELS AND BUTTONS
    TermsAndConditionsStartLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(T_and_C_Start_Text),Style::Font::Regular, Style::Color::black, 40);
    this->addChild(TermsAndConditionsStartLabel);
    
    andLabel =createLabelWith(StringMgr::getInstance()->getStringForKey(T_and_C_And),Style::Font::Regular, Style::Color::black, 40);
    this->addChild(andLabel);
    
    privacyButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(T_and_C_Privacy_Button), 40, true,Style::Color::black);
    privacyButton->setMixPanelButtonName("signupScenePrivacyPolicyButton");
    privacyButton->setDelegate(this);
    this->addChild(privacyButton);
    
    termsButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(T_and_C_Terms_Button), 40, true,Style::Color::black);
    termsButton->setMixPanelButtonName("signupSceneermsButton");
    termsButton->setDelegate(this);
    this->addChild(termsButton);
}

//------------PRIVATE OTHER FUNCTIONS------------

void OnboardingScene::setNewLayout()
{
    float additionYForErrorText = 0.0f;
    
    if(_emailError->isVisible())
    {
        additionYForErrorText = _emailError->getContentSize().height * 1.75f;
        _emailError->setPositionY(_emailTextInput->getPositionY()-_emailError->getContentSize().height/2);
    }
    
    passwordTextInput->setPositionY(_emailTextInput->getPositionY() -passwordTextInput->getContentSize().height*1.9 - additionYForErrorText);
    
    if(_passwordError->isVisible())
    {
        additionYForErrorText = _passwordError->getContentSize().height * 1.75f;
        _passwordError->setPositionY(passwordTextInput->getPositionY()-_passwordError->getContentSize().height/2);
    }
    else
    {
        additionYForErrorText = 0.0f;
    }
    
    pinTextInput->setPositionY(passwordTextInput->getPositionY() -pinTextInput->getContentSize().height*1.9 - additionYForErrorText);
    
    _passwordTitle->setPositionY(passwordTextInput->getPositionY()+(passwordTextInput->getContentSize().height) + (_passwordTitle->getContentSize().height*.6));
    _pinTitle->setPositionY(pinTextInput->getPositionY()+(pinTextInput->getContentSize().height) + (_pinTitle->getContentSize().height*.6));
    
    if(_pinError->isVisible())
    {
        additionYForErrorText = _pinError->getContentSize().height * 1.75f;
        _pinError->setPositionY(pinTextInput->getPositionY()-_pinError->getContentSize().height/2);
    }
    else
    {
        additionYForErrorText = 0.0f;
    }
    
    _pinSubTitle->setPositionY(pinTextInput->getPositionY() - (_pinSubTitle->getContentSize().height*.2)- additionYForErrorText);
    
    signupButton->setCenterPosition(Vec2(visibleSize.width*.75+origin.x, pinTextInput->getPositionY()-signupButton->getContentSize().height*1.1- additionYForErrorText));
    
    cancelButton->setCenterPosition(Vec2(visibleSize.width*.25+origin.x, signupButton->getCenterPosition().y));
    
    //------- CALCULATE AND SET LOCATION OF ITEMS SO THEY ARE CENTERED
    float TermsAndConditionsStartLabelWidth = TermsAndConditionsStartLabel->getContentSize().width;
    float privacyButtonWidth = privacyButton->getContentSize().width;
    float andLabelWidth = andLabel->getContentSize().width;
    float termsButtonWidth = termsButton->getContentSize().width;
    
    float totalWidth = TermsAndConditionsStartLabelWidth + andLabelWidth + privacyButtonWidth + termsButtonWidth;
    float yPosition = signupButton->getPositionY() - (TermsAndConditionsStartLabel->getContentSize().height*2);
    
    //NOTE: Buttons are Layers and the anchor point is (0,0)
    TermsAndConditionsStartLabel->setPosition(origin.x+visibleSize.width/2-totalWidth/2+TermsAndConditionsStartLabelWidth/2,yPosition);
    privacyButton->setCenterPosition(Vec2(TermsAndConditionsStartLabel->getPositionX()+TermsAndConditionsStartLabelWidth/2+privacyButtonWidth/2,yPosition));
    andLabel->setPosition(privacyButton->getPositionX() + privacyButtonWidth + andLabelWidth/2,yPosition);
    termsButton->setCenterPosition(Vec2(andLabel->getPositionX()+andLabelWidth/2+termsButtonWidth/2,yPosition));
    
}

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
    {
        signupButton->setOpacity(255);
        signupButton->setDelegate(this);
    }
    else
    {
        signupButton->setOpacity(80);
        signupButton->setDelegate(nullptr);
    }
}

void OnboardingScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer == _emailTextInput)
    {
        passwordTextInput->focusAndShowKeyboard();
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(_emailTextInput->getText());
        if(!_emailTextInput->inputIsValid())
        {
            _emailError->setVisible(true);
           _emailTextInput->setEditboxHasError();
            setNewLayout();
        }
    }
    else if(inputLayer == passwordTextInput)
        pinTextInput->focusAndShowKeyboard();
    else if(inputLayer == pinTextInput)
        if(_emailTextInput->inputIsValid() && passwordTextInput->inputIsValid() && pinTextInput->inputIsValid())
            signUp();
}

void OnboardingScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    if(inputLayer == _emailTextInput)
    {
        _emailError->setVisible(false);
        _emailTextInput->setEditboxHasError(false);
        setNewLayout();
    }
    else if(inputLayer == passwordTextInput)
    {
        _passwordError->setVisible(false);
        passwordTextInput->setEditboxHasError(false);
        setNewLayout();
    }
    else if(inputLayer == pinTextInput)
    {
        _pinError->setVisible(false);
        pinTextInput->setEditboxHasError(false);
        setNewLayout();
    }
}

void OnboardingScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    if(!_emailTextInput->inputIsValid() && inputLayer == _emailTextInput)
    {
        _emailError->setVisible(true);
        _emailTextInput->setEditboxHasError();
        setNewLayout();
    }
    else if(!passwordTextInput->inputIsValid() && inputLayer == passwordTextInput)
    {
        _passwordError->setVisible(true);
        passwordTextInput->setEditboxHasError();
        setNewLayout();
    }
    else if(!pinTextInput->inputIsValid() && inputLayer == pinTextInput)
    {
        _pinError->setVisible(true);
        pinTextInput->setEditboxHasError();
        setNewLayout();
    }
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
