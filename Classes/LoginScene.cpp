#include "LoginScene.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "OnboardingScene.h"
#include "ConfigStorage.h"
#include "StringStorage.h"
#include "TextInputChecker.h"
#include "TextInputLayer.h"
#include "AudioMixer.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"

USING_NS_CC;

Scene* LoginScene::createScene(long errorCode)
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = errorCode;
    layer->shouldDoAutoLogin = false;
    
    return scene;
}

Scene* LoginScene::createSceneWithAutoLogin()
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = 0;
    layer->shouldDoAutoLogin = true;
    
    return scene;
}

bool LoginScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    CCLOG("Cache folder: %s", FileUtils::getInstance()->getWritablePath().c_str());
    
    AudioMixer::getInstance()->stopBackgroundMusic();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void LoginScene::onEnterTransitionDidFinish()
{
    if(shouldDoAutoLogin)
    {
        CCLOG("Should do autologin!");
        
        UserDefault* def = UserDefault::getInstance();
        std::string username = def->getStringForKey("username", "");
        std::string password = def->getStringForKey("password", "");
        def->flush();
        
        if((username != "")&&(password != ""))
        {
            CCLOG("Doing autologin!");
            
            autoLogin(username, password);
            return;
        }
    }
    else
    {
        UserDefault* def = UserDefault::getInstance();
        def->setStringForKey("password", "");
        def->flush();
    }
    
    addVisualElementsToScene();
    addFunctionalElementsToScene();
    
    
    if(_errorCode !=0)
    {
        ModalMessages::getInstance()->createErrorMessage(_errorCode,nullptr);
    }
    
#ifdef autologin
    BackEndCaller::getInstance()->login("tamas.bonis@azoomee.com", "B0Ta1983!");
#endif
}

//----------------- SCENE SETUP ---------------

void LoginScene::addVisualElementsToScene()
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

void LoginScene::addFunctionalElementsToScene()
{
    addContentLayerToScene();
    addLabelsToLayer();
    addButtonsToLayer();
    addTextBoxesToLayer();
}

void LoginScene::addContentLayerToScene()
{
    loginContent = Layer::create();
    loginContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    loginContent->setPosition(Point(origin.x, origin.y));
    loginContent->setName("loginContent");
    this->addChild(loginContent);
}

void LoginScene::addLabelsToLayer()
{
    auto doYouWantToTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForLoginScene("signUpLoginLabel"), "fonts/azoomee.ttf", 90);
    doYouWantToTitle->setPosition(origin.x+visibleSize.width * 0.5, visibleSize.height*0.65);
    doYouWantToTitle->setColor(Color3B::WHITE);
    loginContent->addChild(doYouWantToTitle);
    
    auto emailTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForLoginScene("emailLabel"), "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForLoginScene("passwordLabel"), "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(passwordTitle);
}

void LoginScene::addTextBoxesToLayer()
{
    UserDefault* def = UserDefault::getInstance();
    std::string username = def->getStringForKey("username", "");
    std::string password = def->getStringForKey("password", "");
    def->flush();
    
    if(username !="") emailNextButton->setVisible(true);
    
    _usernameTextInput = TextInputLayer::createWithSize(Size(736,131), INPUT_IS_EMAIL);
    _usernameTextInput->setCenterPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    _usernameTextInput->setDelegate(this);
    _usernameTextInput->setText(username);
    loginContent->addChild(_usernameTextInput);
    
    _passwordTextInput = TextInputLayer::createWithSize(Size(736,131), INPUT_IS_PASSWORD);
    _passwordTextInput->setCenterPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    _passwordTextInput->setDelegate(this);
    _passwordTextInput->setText(password);
    loginContent->addChild(_passwordTextInput);
}

void LoginScene::addButtonsToLayer()
{
    previewModeButton = ElectricDreamsButton::createCancelButton();
    previewModeButton->setCenterPosition(Vec2(origin.x+previewModeButton->getContentSize().width, visibleSize.height - previewModeButton->getContentSize().height));
    previewModeButton->setDelegate(this);
    loginContent->addChild(previewModeButton);
    
    loginOptionButton = ElectricDreamsButton::createTextAsButton(StringStorage::getInstance()->getStringForLoginScene("loginButton"));
    loginOptionButton->setCenterPosition(Vec2(origin.x+visibleSize.width * 0.5, visibleSize.height*0.35));
    loginOptionButton->setDelegate(this);
    loginContent->addChild(loginOptionButton);
    
    signUpOptionButton = ElectricDreamsButton::createButtonWithText(StringStorage::getInstance()->getStringForLoginScene("signupButton"));
    signUpOptionButton->setCenterPosition(Vec2(origin.x+visibleSize.width * 0.5, visibleSize.height/2));
    signUpOptionButton->setDelegate(this);
    loginContent->addChild(signUpOptionButton);
    
    emailBackButton = ElectricDreamsButton::createBackButton();
    emailBackButton->setCenterPosition(Vec2(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5));
    emailBackButton->setDelegate(this);
    loginContent->addChild(emailBackButton);
    
    emailNextButton = ElectricDreamsButton::createNextButton();
    emailNextButton->setCenterPosition(Vec2(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5));
    emailNextButton->setDelegate(this);
    emailNextButton->setVisible(false);
    loginContent->addChild(emailNextButton);
    
    passwordBackButton = ElectricDreamsButton::createBackButton();
    passwordBackButton->setCenterPosition(Vec2(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5));
    passwordBackButton->setDelegate(this);
    loginContent->addChild(passwordBackButton);
    
    loginButton = ElectricDreamsButton::createNextButton();
    loginButton->setCenterPosition(Vec2(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5));
    loginButton->setDelegate(this);
    loginButton->setVisible(false);
    loginButton->setScale(1.2);
    loginContent->addChild(loginButton);
}

//------------------- Button Functions -----------------------

void LoginScene::disableButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(button);
}

void LoginScene::enableButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(button);
}

void LoginScene::setTextInputFocus(TextInputLayer* textInputLayer)
{
    textInputLayer->focusAndShowKeyboard();
}

void LoginScene::switchToSignupScene(ElectricDreamsButton* button)
{
    auto _OnboardingScene = OnboardingScene::createScene(0);
    Director::getInstance()->replaceScene(_OnboardingScene);
}

void LoginScene::moveToAndSetupEmailScreen(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2);
    auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableButton, this,button));
    auto setTextInputFocusCallback = CallFunc::create(CC_CALLBACK_0(LoginScene::setTextInputFocus, this,_usernameTextInput));
    
    auto sequence = Sequence::create(action, setTextInputFocusCallback, enableButtonCallback, NULL);
    loginContent->runAction(sequence);
    
    _passwordTextInput->setText("");
}

void LoginScene::moveToBackToSelectionScreen(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2);
    auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableButton, this,button));
    
    auto sequence = Sequence::create(action, enableButtonCallback, NULL);
    loginContent->runAction(sequence);
    
    _usernameTextInput->setText("");
}

void LoginScene::moveToAndSetupPasswordScreen(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2);
    auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableButton, this,button));
    auto setTextInputFocusCallback = CallFunc::create(CC_CALLBACK_0(LoginScene::setTextInputFocus, this,_passwordTextInput));
    
    auto sequence = Sequence::create(action, setTextInputFocusCallback, enableButtonCallback, NULL);
    loginContent->runAction(sequence);
}

void LoginScene::login(ElectricDreamsButton* button)
{
    std::string username = _usernameTextInput->getText();
    std::string password = _passwordTextInput->getText();

    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

void LoginScene::autoLogin(std::string username, std::string password)
{
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

void LoginScene::moveToPreviewScene()
{
    HQHistoryManager::getInstance()->emptyHistory();
    auto baseScene = BaseScene::createScene();
    Director::getInstance()->replaceScene(baseScene);
}

//----------------------- Delegate Functions ----------------------------

void LoginScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(inputLayer == _usernameTextInput)
        emailNextButton->setVisible(isValid);
    else if(inputLayer == _passwordTextInput)
        loginButton->setVisible(isValid);

}

void LoginScene::buttonPressed(ElectricDreamsButton* button)
{
    disableButton(button);
    
    if(button == loginOptionButton) this->moveToAndSetupEmailScreen(button);
    else if(button == signUpOptionButton) this->switchToSignupScene(button);
    else if(button == emailBackButton) this->moveToBackToSelectionScreen(button);
    else if(button == emailNextButton) this->moveToAndSetupPasswordScreen(button);
    else if(button == passwordBackButton) this->moveToAndSetupEmailScreen(button);
    else if(button == loginButton) this->login(button);
    else if(button == previewModeButton) this->moveToPreviewScene();
    
}
