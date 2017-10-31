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
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
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
        _passwordTextInput->setEditboxVisibility(false);
        _pinTextInput->setEditboxVisibility(false);
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), this);
    }
    else
        _emailTextInput->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------

void OnboardingScene::addBackgroundandDecoration()
{
    Layer* background = createTopGradientAndParticles();
    background->setPosition(_origin.x,_origin.y);
    this->addChild(background);
}

void OnboardingScene::addProgressIndicator()
{
    _progressIndicatior = Sprite::create("res/decoration/progress1.png");
    _progressIndicatior->setPosition(_origin.x+_visibleSize.width/2,_origin.y+_visibleSize.height - _progressIndicatior->getContentSize().height * 1.5f);
    this->addChild(_progressIndicatior);
}

void OnboardingScene::addMainTitleToScene()
{
    _mainTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_TITLE_LABEL));
    _mainTitle->setPositionY(_progressIndicatior->getPositionY() - _progressIndicatior->getContentSize().height - _mainTitle->getContentSize().height/2);
    this->addChild(_mainTitle);
}

void OnboardingScene::addTextboxScene()
{
    float textInputWidth = _visibleSize.width * 0.80f;
    
    _emailTextInput = TextInputLayer::createWithSize(Size(textInputWidth,160), INPUT_IS_EMAIL);
    _emailTextInput->setPositionY(_mainTitle->getPositionY()-_emailTextInput->getContentSize().height*2.1);
    _emailTextInput->setDelegate(this);
    _emailTextInput->setText(FlowDataSingleton::getInstance()->getUserName());
    this->addChild(_emailTextInput);
    
    _passwordTextInput = TextInputLayer::createWithSize(Size(textInputWidth,160), INPUT_IS_NEW_PASSWORD);
    _passwordTextInput->setDelegate(this);
    this->addChild(_passwordTextInput);
    
    _pinTextInput = TextInputLayer::createWithSize(Size(600,160), INPUT_IS_PIN);
    _pinTextInput->setDelegate(this);
    this->addChild(_pinTextInput);
}

void OnboardingScene::addLabelsToScene()
{
    _emailTitle =  createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL));
    _emailTitle->setPositionY(_emailTextInput->getPositionY()+(_emailTextInput->getContentSize().height) + (_emailTitle->getContentSize().height * 0.6f));
    this->addChild(_emailTitle);
    
    _emailError = createLabelBodyCentred("Email doesn't look correct - please check",Style::Color::watermelon);
    _emailError->setLineSpacing(20);
    _emailError->setVisible(false);
    this->addChild(_emailError);
    
    _passwordTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PASSWORD_LABEL));
    this->addChild(_passwordTitle);
    
    _passwordError = createLabelBodyCentred("Password must be at least 6 characters",Style::Color::watermelon);
    _passwordError->setLineSpacing(20);
    _passwordError->setVisible(false);
    this->addChild(_passwordError);
    
    _pinTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL));
    this->addChild(_pinTitle);
    
    _pinError = createLabelBodyCentred("PIN must be 4 digits",Style::Color::watermelon);
    _pinError->setLineSpacing(20);
    _pinError->setVisible(false);
    this->addChild(_pinError);
    
    _pinSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL),Style::Color::black);
    _pinSubTitle->setLineSpacing(20);
    this->addChild(_pinSubTitle);
}

void OnboardingScene::addButtonsScene()
{
    _signupButton = ElectricDreamsButton::createGreenButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE), _visibleSize.width/3);
    _signupButton->setMixPanelButtonName("signupSceneContinueButton");
    _signupButton->setOpacity(80);
    this->addChild(_signupButton);
    
    _cancelButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL), 75, true, Color3B::BLACK);
    _cancelButton->setDelegate(this);
    _cancelButton->setMixPanelButtonName("signupSceneCancelButton");
    this->addChild(_cancelButton);
}

void OnboardingScene::addTermsAndConditionsToScene()
{
    //---------- CREATE AND ADD LABELS AND BUTTONS
    _TermsAndConditionsStartLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(T_and_C_Start_Text),Style::Font::Regular, Style::Color::black, 40);
    this->addChild(_TermsAndConditionsStartLabel);
    
    _andLabel =createLabelWith(StringMgr::getInstance()->getStringForKey(T_and_C_And),Style::Font::Regular, Style::Color::black, 40);
    this->addChild(_andLabel);
    
    _privacyButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(T_and_C_Privacy_Button), 40, true,Style::Color::black);
    _privacyButton->setMixPanelButtonName("signupScenePrivacyPolicyButton");
    _privacyButton->setDelegate(this);
    this->addChild(_privacyButton);
    
    _termsButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(T_and_C_Terms_Button), 40, true,Style::Color::black);
    _termsButton->setMixPanelButtonName("signupSceneermsButton");
    _termsButton->setDelegate(this);
    this->addChild(_termsButton);
}

//------------PRIVATE OTHER FUNCTIONS------------

void OnboardingScene::textInputHasError(TextInputLayer *textInputLayer, cocos2d::Label* errorLabel, bool hasError)
{
    errorLabel->setVisible(hasError);
    textInputLayer->setEditboxHasError(hasError);
    setNewLayout();
}

void OnboardingScene::setNewLayout()
{
    float additionYForErrorText = 0.0f;
    
    if(_emailError->isVisible())
    {
        additionYForErrorText = _emailError->getContentSize().height * 1.75f;
        _emailError->setPositionY(_emailTextInput->getPositionY()-_emailError->getContentSize().height/2);
    }
    
    _passwordTextInput->setPositionY(_emailTextInput->getPositionY() - _passwordTextInput->getContentSize().height * 1.9f - additionYForErrorText);
    
    if(_passwordError->isVisible())
    {
        additionYForErrorText = _passwordError->getContentSize().height * 1.75f;
        _passwordError->setPositionY(_passwordTextInput->getPositionY()-_passwordError->getContentSize().height/2);
    }
    else
    {
        additionYForErrorText = 0.0f;
    }
    
    _pinTextInput->setPositionY(_passwordTextInput->getPositionY() - _pinTextInput->getContentSize().height * 1.9f - additionYForErrorText);
    
    _passwordTitle->setPositionY(_passwordTextInput->getPositionY()+(_passwordTextInput->getContentSize().height) + (_passwordTitle->getContentSize().height * 0.6f));
    _pinTitle->setPositionY(_pinTextInput->getPositionY() + (_pinTextInput->getContentSize().height) + (_pinTitle->getContentSize().height * 0.6f));
    
    if(_pinError->isVisible())
    {
        additionYForErrorText = _pinError->getContentSize().height * 1.75f;
        _pinError->setPositionY(_pinTextInput->getPositionY()-_pinError->getContentSize().height/2);
    }
    else
    {
        additionYForErrorText = 0.0f;
    }
    
    _pinSubTitle->setPositionY(_pinTextInput->getPositionY() - (_pinSubTitle->getContentSize().height*.2)- additionYForErrorText);
    
    _signupButton->setCenterPosition(Vec2(_visibleSize.width * 0.75f + _origin.x, _pinTextInput->getPositionY()- _signupButton->getContentSize().height * 1.1f - additionYForErrorText));
    
    _cancelButton->setCenterPosition(Vec2(_visibleSize.width * 0.25f + _origin.x, _signupButton->getCenterPosition().y));
    
    //------- CALCULATE AND SET LOCATION OF ITEMS SO THEY ARE CENTERED
    float TermsAndConditionsStartLabelWidth = _TermsAndConditionsStartLabel->getContentSize().width;
    float privacyButtonWidth = _privacyButton->getContentSize().width;
    float andLabelWidth = _andLabel->getContentSize().width;
    float termsButtonWidth = _termsButton->getContentSize().width;
    
    float totalWidth = TermsAndConditionsStartLabelWidth + andLabelWidth + privacyButtonWidth + termsButtonWidth;
    float yPosition = _signupButton->getPositionY() - (_TermsAndConditionsStartLabel->getContentSize().height*2);
    
    //NOTE: Buttons are Layers and the anchor point is (0,0)
    _TermsAndConditionsStartLabel->setPosition(_origin.x + _visibleSize.width/2-totalWidth/2+TermsAndConditionsStartLabelWidth/2,yPosition);
    _privacyButton->setCenterPosition(Vec2(_TermsAndConditionsStartLabel->getPositionX()+TermsAndConditionsStartLabelWidth/2+privacyButtonWidth/2,yPosition));
    _andLabel->setPosition(_privacyButton->getPositionX() + privacyButtonWidth + andLabelWidth/2,yPosition);
    _termsButton->setCenterPosition(Vec2(_andLabel->getPositionX() + andLabelWidth/2 + termsButtonWidth/2,yPosition));
    
}

void OnboardingScene::signUp()
{
    ModalMessages::getInstance()->startLoading();
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(_emailTextInput->getText());
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->registerParent(_emailTextInput->getText(), _passwordTextInput->getText(), _pinTextInput->getText());
}

//----------------------- Delegate Functions ----------------------------

void OnboardingScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(_emailTextInput->inputIsValid() && _passwordTextInput->inputIsValid() && _pinTextInput->inputIsValid())
    {
        _signupButton->setOpacity(255);
        _signupButton->setDelegate(this);
    }
    else
    {
        _signupButton->setOpacity(80);
        _signupButton->setDelegate(nullptr);
    }
}

void OnboardingScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer == _emailTextInput)
    {
        _passwordTextInput->focusAndShowKeyboard();
        AnalyticsSingleton::getInstance()->registerAzoomeeEmail(_emailTextInput->getText());
        if(!_emailTextInput->inputIsValid())
        {
            textInputHasError(_emailTextInput, _emailError);
        }
    }
    else if(inputLayer == _passwordTextInput)
    {
        _pinTextInput->focusAndShowKeyboard();
    }
    else if(inputLayer == _pinTextInput)
    {
        //if all Text boxes do not have errors
        if(_emailTextInput->inputIsValid() && _passwordTextInput->inputIsValid() && _pinTextInput->inputIsValid())
        {
            signUp();
        }
    }
}

void OnboardingScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    if(inputLayer == _emailTextInput)
    {
        textInputHasError(_emailTextInput, _emailError, false);
    }
    else if(inputLayer == _passwordTextInput)
    {
        textInputHasError(_passwordTextInput, _passwordError, false);
    }
    else if(inputLayer == _pinTextInput)
    {
        textInputHasError(_pinTextInput, _pinError, false);
    }
}

void OnboardingScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    if(!_emailTextInput->inputIsValid() && inputLayer == _emailTextInput)
    {
        textInputHasError(_emailTextInput, _emailError);
    }
    else if(!_passwordTextInput->inputIsValid() && inputLayer == _passwordTextInput)
    {
        textInputHasError(_passwordTextInput, _passwordError);
    }
    else if(!_pinTextInput->inputIsValid() && inputLayer == _pinTextInput)
    {
        textInputHasError(_pinTextInput, _pinError);
    }
}

void OnboardingScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == _signupButton)
    {
        signUp();
    }
    else if(button == _cancelButton)
    {
        BackEndCaller::getInstance()->anonymousDeviceLogin();
    }
    else if(button == _privacyButton)
    {
        ModalWebview::createWithURL(Url::PrivacyPolicy);
    }
    else if(button == _termsButton)
    {
        ModalWebview::createWithURL(Url::TermsOfUse);
    }
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
        _passwordTextInput->setEditboxVisibility(true);
        _pinTextInput->setEditboxVisibility(true);
        _emailTextInput->focusAndShowKeyboard();
    }
}

NS_AZOOMEE_END
