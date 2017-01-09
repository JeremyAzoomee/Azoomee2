#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "OnboardingScene.h"
#include "ConfigStorage.h"
#include "StringStorage.h"
#include "TextInputChecker.h"

#include "TextInputLayer.h"

USING_NS_CC;

Scene* LoginScene::createScene(long errorCode)
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    
    layer->_errorCode = errorCode;
    
    return scene;
}

bool LoginScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualElementsToScene();
    addFunctionalElementsToScene();
    
    return true;
}

//-------------------------------------------All methods beyond this line are called internally-------------------------------------------------------

void LoginScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        handleErrorCode(_errorCode);
    }
    
#ifdef autologin
        BackEndCaller::getInstance()->login("klaas+ci@azoomee.com", "test1234");
#endif
}

void LoginScene::handleErrorCode(long errorCode)
{
    ModalMessages::getInstance()->createMessageWithSingleButton("ERROR", StringUtils::format("Error Code:%ld",errorCode), "OK");
}

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
    addTextBoxesToLayer();
    addButtonsToLayer();
}

void LoginScene::addLabelsToLayer()
{
    auto emailTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForLoginScene("emailLabel"), "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForLoginScene("passwordLabel"), "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(passwordTitle);
}

void LoginScene::addContentLayerToScene()
{
    loginContent = Layer::create();
    loginContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    loginContent->setPosition(Point(origin.x, origin.y));
    loginContent->setName("loginContent");
    this->addChild(loginContent);
}

void LoginScene::addTextBoxesToLayer()
{
    _editName = TextInputLayer::createWithSize(Size(736,131), INPUT_IS_EMAIL);
    _editName->setCenterPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    _editName->setDelegate(this);
    loginContent->addChild(_editName);
    
    _editPassword = TextInputLayer::createWithSize(Size(736,131), INPUT_IS_PASSWORD);
    _editPassword->setCenterPosition(Vec2(origin.x+visibleSize.width * 2.5, origin.y+visibleSize.height*0.5));
    _editPassword->setDelegate(this);
    loginContent->addChild(_editPassword);
}

void LoginScene::addButtonsToLayer()
{
    auto signUpButton = ui::Button::create();
    signUpButton->setTitleText(StringStorage::getInstance()->getStringForLoginScene("signupButton"));
    signUpButton->setTitleFontName("fonts/azoomee.ttf");
    signUpButton->setPosition(Vec2(origin.x+visibleSize.width * 0.5, origin.y+visibleSize.height*0.6));
    signUpButton->setTitleFontSize(90);
    signUpButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                break;
            }
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                auto _OnboardingScene = OnboardingScene::createScene(0);
                Director::getInstance()->replaceScene(_OnboardingScene);
                break;
        }
    });
    loginContent->addChild(signUpButton);
    
    loginButton = ui::Button::create();
    loginButton->setTitleText(StringStorage::getInstance()->getStringForLoginScene("loginButton"));
    loginButton->setTitleFontName("fonts/azoomee.ttf");
    loginButton->setPosition(Vec2(origin.x+visibleSize.width * 0.5, origin.y+visibleSize.height*0.4));
    loginButton->setTitleFontSize(90);
    loginButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                break;
            }
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                if ( loginContent->getNumberOfRunningActions() == 0 )
                    moveLoginToEmailScreen(loginButton);
                break;
        }
    });
    loginContent->addChild(loginButton);
    
    emailBackButton = Sprite::create("res/login/back_btn.png");
    emailBackButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(emailBackButton);
    loginContent->addChild(emailBackButton);
    
    emailNextButton = Sprite::create("res/login/next_btn.png");
    emailNextButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    addListenerToButton(emailNextButton);
    emailNextButton->setVisible(false);
    loginContent->addChild(emailNextButton);
    
    passwordBackButton = Sprite::create("res/login/back_btn.png");
    passwordBackButton->setPosition(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5);
    addListenerToButton(passwordBackButton);
    loginContent->addChild(passwordBackButton);
    
    passwordNextButton = Sprite::create("res/login/next_btn.png");
    passwordNextButton->setPosition(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5);
    addListenerToButton(passwordNextButton);
    passwordNextButton->setVisible(false);
    passwordNextButton->setScale(1.2);
    loginContent->addChild(passwordNextButton);
}

void LoginScene::addListenerToButton(cocos2d::Sprite *spriteImage)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
        
        if(rect.containsPoint(locationInNode) && spriteImage->isVisible())
        {
            if(spriteImage == emailBackButton) this->moveToBackFirstScreenEnableLogin(spriteImage);
            if(spriteImage == emailNextButton) this->moveToPasswordScreen(spriteImage);
            if(spriteImage == passwordBackButton) this->moveToEmailScreen(spriteImage);
            if(spriteImage == passwordNextButton) this->login();
            
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void LoginScene::disableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(button);
}

void LoginScene::enableMoveButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(button);
}

void LoginScene::moveLoginToEmailScreen(ui::Button* button)
{
    loginContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
    
    _editName->focusAndShowKeyboard();
}

void LoginScene::moveToBackFirstScreenEnableLogin(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    loginContent->runAction(sequence);
}

void LoginScene::moveToPasswordScreen(Node* button)
{
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    loginContent->runAction(sequence);
}

void LoginScene::moveToEmailScreen(Node* button)
{
    std::string username = _editName->getText();
    std::string password = _editPassword->getText();
    disableMoveButton(button);
    
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(LoginScene::enableMoveButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    loginContent->runAction(sequence);
    
    _editPassword->setText("");
    passwordNextButton->setVisible(false);
    
    _editName->focusAndShowKeyboard();
}

void LoginScene::login()
{
    std::string username = _editName->getText();
    std::string password = _editPassword->getText();

    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

void LoginScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(inputLayer == _editName)
        emailNextButton->setVisible(isValid);
    else if(inputLayer == _editPassword)
        passwordNextButton->setVisible(isValid);

}
