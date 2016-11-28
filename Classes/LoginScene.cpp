#include "LoginScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoginScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
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
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
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
    addTextBoxesToLayer();
    addButtonsToLayer();
}

void LoginScene::addContentLayerToScene()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //we add an additional layer that will slide on the screen between the username and the password.
    //we add all input elements on this layer.
    
    auto loginContent = Layer::create();
    loginContent->setContentSize(Size(visibleSize.width * 2, visibleSize.height));
    loginContent->setPosition(Point(origin.x, origin.y));
    this->addChild(loginContent);
}

void LoginScene::addTextBoxesToLayer()
{
    std::string pNormalSprite = "res/editBg.png";
    auto _editName = ui::EditBox::create(Size(400,30), ui::Scale9Sprite::create(pNormalSprite));
    _editName->setColor(Color3B::WHITE);
    _editName->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.75));
    _editName->setFontName("fonts/arial.ttf");
    _editName->setFontSize(20);
    _editName->setFontColor(Color3B::BLUE);
    _editName->setPlaceHolder("username");
    _editName->setPlaceholderFontColor(Color3B::BLUE);
    _editName->setMaxLength(50);
    _editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editName->setTag(1);
    addChild(_editName);
    
    auto _editPassword = ui::EditBox::create(Size(400,30), "res/editBg.png");
    _editPassword->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.6));
    _editPassword->setFont("fonts/arial.ttf", 20);
    _editPassword->setFontColor(Color3B::BLUE);
    _editPassword->setPlaceHolder("password");
    _editPassword->setMaxLength(50);
    _editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _editPassword->setTag(2);
    addChild(_editPassword);
}

void LoginScene::addButtonsToLayer()
{
    auto loginButton = Sprite::create("res/loginButton.png");
    loginButton->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height*0.45 + origin.y));
    loginButton->setScale(2);
    this->addChild(loginButton);
}

void LoginScene::addListenerToLoginButton()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch *touch, Event *event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            //auto usernameString = target->getParent()->getChildByTag(1)->getStringValue();
            
            ui::EditBox *myUserField = (ui::EditBox *)target->getParent()->getChildByTag(1);
            std::string username = myUserField->getText();
            
            ui::EditBox *myPassField = (ui::EditBox *)target->getParent()->getChildByTag(2);
            std::string password = myPassField->getText();
            
            CCLOG("%s - %s", username.c_str(), password.c_str());
            
            if(username == "")
            {
                username = "klaas+ci@azoomee.com";
                password = "test1234";
            }
            
            auto myBeCommSt = BeCommSt::getInstance();
            myBeCommSt->login(username, password);

            return true;
        }
        
        return false;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, loginButton);
}

void LoginScene::addListenerToNextScreenButton()
{
    
}
