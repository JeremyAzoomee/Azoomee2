#include "OnboardingScene.h"
#include "BackEndCaller.h"
#include "LoginScene.h"
#include "StringMgr.h"
#include "AnalyticsSingleton.h"
#include "MessageBox.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"

USING_NS_CC;

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
    
    AnalyticsSingleton::getInstance()->OnboardingStartEvent();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualElementsToScene();
    addFunctionalElementsToScene();
    
    return true;
}

void OnboardingScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        MessageBox::createWith(_errorCode, editBox_email, nullptr);
    }
    
    editBox_email->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------

void OnboardingScene::addVisualElementsToScene()
{
    addGlowToScreen(this, 1);
    addSideWiresToScreen(this, 0, 2);
}

void OnboardingScene::addFunctionalElementsToScene()
{
    addContentLayerToScene();
    addLabelsToLayer();
    addTextBoxesToLayer();
    addButtonsToLayer();
}

void OnboardingScene::addLabelsToLayer()
{
    auto emailTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL));
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    onboardingContent->addChild(emailTitle);
    
    auto passwordTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PASSWORD_LABEL));
    passwordTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    onboardingContent->addChild(passwordTitle);
    
    auto pinTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL));
    pinTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    onboardingContent->addChild(pinTitle);
    
    auto pinDetail = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL));
    pinDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
    onboardingContent->addChild(pinDetail);
}

void OnboardingScene::addContentLayerToScene()
{
    onboardingContent = Layer::create();
    onboardingContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    onboardingContent->setPosition(Point(origin.x, origin.y));
    onboardingContent->setName("onboardingContent");
    this->addChild(onboardingContent);
}

void OnboardingScene::addTextBoxesToLayer()
{
    editBox_email = TextInputLayer::createWithSize(Size(1200,131), INPUT_IS_EMAIL);
    editBox_email->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    editBox_email->setDelegate(this);
    onboardingContent->addChild(editBox_email);
    
    editBox_password = TextInputLayer::createWithSize(Size(1200,131), INPUT_IS_PASSWORD);
    editBox_password->setCenterPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    editBox_password->setDelegate(this);
    onboardingContent->addChild(editBox_password);
    
    editBox_pin = TextInputLayer::createWithSize(Size(400,131), INPUT_IS_PIN);
    editBox_pin->setCenterPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    editBox_pin->setDelegate(this);
    onboardingContent->addChild(editBox_pin);
}

void OnboardingScene::addButtonsToLayer()
{
    buttonBackEmail = ElectricDreamsButton::createBackButton();
    buttonBackEmail->setCenterPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.5));
    buttonBackEmail->setDelegate(this);
    onboardingContent->addChild(buttonBackEmail);
    
    buttonNextEmail = ElectricDreamsButton::createNextButton();
    buttonNextEmail->setCenterPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5));
    buttonNextEmail->setDelegate(this);
    buttonNextEmail->setVisible(false);
    onboardingContent->addChild(buttonNextEmail);
    
    buttonBackPassword = ElectricDreamsButton::createBackButton();
    buttonBackPassword->setCenterPosition(Vec2(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5));
    buttonBackPassword->setDelegate(this);
    onboardingContent->addChild(buttonBackPassword);

    buttonNextPassword = ElectricDreamsButton::createNextButton();
    buttonNextPassword->setCenterPosition(Vec2(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5));
    buttonNextPassword->setDelegate(this);
    buttonNextPassword->setVisible(false);
    onboardingContent->addChild(buttonNextPassword);
    
    buttonBackPin = ElectricDreamsButton::createBackButton();
    buttonBackPin->setCenterPosition(Vec2(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5));
    buttonBackPin->setDelegate(this);
    onboardingContent->addChild(buttonBackPin);

    buttonSignUp = ElectricDreamsButton::createNextButton();
    buttonSignUp->setCenterPosition(Vec2(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5));
    buttonSignUp->setDelegate(this);
    buttonSignUp->setScale(1.2);
    buttonSignUp->setVisible(false);
    onboardingContent->addChild(buttonSignUp);
}

//------------------- Button Functions -----------------------

void OnboardingScene::disableButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(button);
}

void OnboardingScene::enableButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(button);
}

void OnboardingScene::setTextInputFocus(TextInputLayer* textInputLayer)
{
    textInputLayer->focusAndShowKeyboard();
}

void OnboardingScene::closeOnboarding()
{
    auto _loginScene = LoginScene::createScene(0);
    Director::getInstance()->replaceScene(_loginScene);
}

void OnboardingScene::moveToAndSetupEmailScreen(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2);
    auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::enableButton, this,button));
    auto setTextInputFocusCallback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::setTextInputFocus, this,editBox_password));
    
    auto sequence = Sequence::create(action, setTextInputFocusCallback, enableButtonCallback, NULL);
    onboardingContent->runAction(sequence);
    
    editBox_password->setText("");
    buttonNextPassword->setVisible(false);
    
    cleanPasswordScreen();
    cleanPinScreen();
}

void OnboardingScene::moveToAndSetupPasswordScreen(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2);
    auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::enableButton, this,button));
    auto setTextInputFocusCallback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::setTextInputFocus, this,editBox_password));
    
    auto sequence = Sequence::create(action, setTextInputFocusCallback, enableButtonCallback, NULL);
    onboardingContent->runAction(sequence);
    
    cleanPinScreen();
}

void OnboardingScene::moveToAndSetupPinScreen(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width * 2 + origin.x, origin.y)), 2);
    auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::enableButton, this,button));
    auto setTextInputFocusCallback = CallFunc::create(CC_CALLBACK_0(OnboardingScene::setTextInputFocus, this,editBox_pin));
    
    auto sequence = Sequence::create(action, setTextInputFocusCallback, enableButtonCallback, NULL);
    onboardingContent->runAction(sequence);
}

void OnboardingScene::cleanPasswordScreen()
{
    editBox_password->setText("");
    buttonNextPassword->setVisible(false);
}

void OnboardingScene::cleanPinScreen()
{
    editBox_pin->setText("");
    buttonSignUp->setVisible(false);
}

void OnboardingScene::signUp()
{
    std::string username = editBox_email->getText();
    std::string password = editBox_password->getText();
    std::string pin = editBox_pin->getText();
    
    BackEndCaller::getInstance()->registerParent(username, password,pin);
}

//----------------------- Delegate Functions ----------------------------

void OnboardingScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(inputLayer == editBox_email)
        buttonNextEmail->setVisible(isValid);
    else if(inputLayer == editBox_password)
        buttonNextPassword->setVisible(isValid);
    else if(inputLayer == editBox_pin)
        buttonSignUp->setVisible(isValid);
}

void OnboardingScene::buttonPressed(ElectricDreamsButton* button)
{
    disableButton(button);
    
    if(button == buttonBackEmail)
        closeOnboarding();
    else if(button == buttonNextEmail)
    {
        moveToAndSetupPasswordScreen(button);
        AnalyticsSingleton::getInstance()->OnboardingEmailSubmittedEvent(editBox_email->getText());
    }
    else if(button == buttonBackPassword)
        moveToAndSetupEmailScreen(button);
    else if(button == buttonNextPassword)
    {
        moveToAndSetupPinScreen(button);
        AnalyticsSingleton::getInstance()->OnboardingPasswordSubmittedEvent();
    }
    else if(button == buttonBackPin)
        moveToAndSetupPasswordScreen(button);
    else if(button == buttonSignUp)
    {
        AnalyticsSingleton::getInstance()->OnboardingPinSubmittedEvent();
        signUp();
    }
}
