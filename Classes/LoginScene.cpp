#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"

USING_NS_CC;

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
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


void LoginScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

void LoginScene::addVisualElementsToScene()
{
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
    auto emailTitle = Label::createWithTTF("Login email", "fonts/azoomee.ttf", 90);
    emailTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    emailTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(emailTitle);
    
    auto passwordTitle = Label::createWithTTF("Login password", "fonts/azoomee.ttf", 90);
    passwordTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    passwordTitle->setColor(Color3B(28, 244, 244));
    loginContent->addChild(passwordTitle);
}

void LoginScene::addContentLayerToScene()
{
    //we add an additional layer that will slide on the screen between the username and the password.
    //we add all input elements on this layer.
    
    loginContent = Layer::create();
    loginContent->setContentSize(Size(visibleSize.width * 2, visibleSize.height));
    loginContent->setPosition(Point(origin.x, origin.y));
    loginContent->setName("loginContent");
    this->addChild(loginContent);
}

void LoginScene::addTextBoxesToLayer()
{
    auto _editName = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editName->setColor(Color3B::WHITE);
    _editName->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    _editName->setFont("fonts/azoomee.ttf", 90);
    _editName->setFontColor(Color3B::WHITE);
    _editName->setMaxLength(100);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editName->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
    _editName->setName("usernameField");
    loginContent->addChild(_editName);
    
    auto _editPassword = ui::EditBox::create(Size(736,131), "res/login/textarea_bg.png");
    _editPassword->setPosition(Vec2(origin.x+visibleSize.width * 1.5, origin.y+visibleSize.height*0.5));
    _editPassword->setFont("fonts/azoomee.ttf", 90);
    _editPassword->setFontColor(Color3B::WHITE);
    _editPassword->setMaxLength(50);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPassword->setName("passwordField");
    loginContent->addChild(_editPassword);
}

void LoginScene::addButtonsToLayer()
{
    auto loginButton = Sprite::create("res/login/next_btn.png");
    loginButton->setPosition(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5);
    loginButton->setScale(1.2);
    loginButton->setTag(0);
    addListenerToButton(loginButton);
    loginContent->addChild(loginButton);
    
    auto nextButton = Sprite::create("res/login/next_btn.png");
    nextButton->setPosition(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5);
    nextButton->setTag(1);
    addListenerToButton(nextButton);
    loginContent->addChild(nextButton);
    
    auto backButton = Sprite::create("res/login/back_btn.png");
    backButton->setPosition(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5);
    backButton->setTag(2);
    addListenerToButton(backButton);
    loginContent->addChild(backButton);
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
        
        if(rect.containsPoint(locationInNode))
        {
            int buttonTag = spriteImage->getTag();
            
            switch(buttonTag)
            {
                case(0): this->loginButtonAction(); break;
                case(1): this->nextButtonAction(); break;
                case(2): this->backButtonAction(); break;
            }
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, spriteImage);
}

void LoginScene::loginButtonAction()
{
    std::string username = ((ui::EditBox *)loginContent->getChildByName("usernameField"))->getText();
    std::string password = ((ui::EditBox *)loginContent->getChildByName("passwordField"))->getText();
    
    //FOR DEBUG PURPOSES ONLY, PLEASE REMOVE WHEN GETTING INTO PRODUCTION
    
    if(username == "aaa")
    {
        username = "klaas+ci@azoomee.com";
        password = "test1234";
    }

    //DELETE UNTIL THIS POINT IN PRODUCTION

    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->login(username, password);
}

void LoginScene::nextButtonAction()
{
    loginContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2));
}

void LoginScene::backButtonAction()
{
    ((ui::EditBox *)loginContent->getChildByName("passwordField"))->setText("");
    loginContent->runAction(EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2));
}
