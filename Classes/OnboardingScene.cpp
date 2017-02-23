#include "OnboardingScene.h"
#include "BackEndCaller.h"
#include "LoginScene.h"
#include "StringMgr.h"
#include "AnalyticsSingleton.h"
#include "MessageBox.h"

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
    auto bg = Sprite::create("res/mainhub/bg_glow.png");
    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(bg);
    
    auto leftBg = Sprite::create("res/login/wire_left.png");
    leftBg->setPosition(0 + leftBg->getContentSize().width / 2,0);
    this->addChild(leftBg);
    
    auto rightBg = Sprite::create("res/login/wire_right.png");
    rightBg->setPosition(origin.x + visibleSize.width - rightBg->getContentSize().width / 2, 0);
    this->addChild(rightBg);
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
    auto emailTitle = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_EMAIL_LABEL), "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PASSWORD_LABEL), "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(passwordTitle);
    
    auto pinTitle = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_LABEL), "fonts/azoomee.ttf", 90);
    pinTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    pinTitle->setColor(Color3B(28, 244, 244));
    onboardingContent->addChild(pinTitle);
    
    auto pinDetail = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(ONBOARDINGSCENE_PIN_SUB_LABEL), "fonts/azoomee.ttf", 60);
    pinDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
    pinDetail->setColor(Color3B::WHITE);
    pinDetail->setHorizontalAlignment(TextHAlignment::CENTER);
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
    editBox_email = TextInputLayer::createWithSize(Size(736,131), INPUT_IS_EMAIL);
    editBox_email->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    editBox_email->setDelegate(this);
    onboardingContent->addChild(editBox_email);
    
    editBox_password = TextInputLayer::createWithSize(Size(736,131), INPUT_IS_PASSWORD);
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
        AnalyticsSingleton::getInstance()->OnboardingEmailSubmittedEvent();
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
