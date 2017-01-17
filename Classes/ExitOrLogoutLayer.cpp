#include "ExitOrLogoutLayer.h"
#include "LoginScene.h"

bool ExitOrLogoutLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    
    createAndFadeInLayer();
    addUIObjects();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void ExitOrLogoutLayer::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(0,0,0,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    //backgroundLayer->setPosition(origin.x, origin.y);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void ExitOrLogoutLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

void ExitOrLogoutLayer::addUIObjects()
{
    
    cancelButton = ElectricDreamsButton::createCancelButton();
    cancelButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.3));
    cancelButton->setDelegate(this);
    backgroundLayer->addChild(cancelButton);
    
    exitButton = ElectricDreamsButton::createButtonWithText("Exit App");
    exitButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.8));
    exitButton->setDelegate(this);
    backgroundLayer->addChild(exitButton);
    
    logoutButton = ElectricDreamsButton::createButtonWithText("Log Out");
    logoutButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.6));
    logoutButton->setDelegate(this);
    backgroundLayer->addChild(logoutButton);
    
}

//---------------------- Actions -----------------

void ExitOrLogoutLayer::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

//---------------------- public Functions After Setup -----------------------------

void ExitOrLogoutLayer::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

Vec2 ExitOrLogoutLayer::getCenterPosition()
{
    return Vec2(this->getPositionX() + this->getContentSize().width/2, this->getPositionY() + this->getContentSize().height/2);
}

//----------------------- Delegate Functions ----------------------------

void ExitOrLogoutLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
        removeSelf();
    else if(button == exitButton)
    {
        Director::getInstance()->end();
    }
    else if(button == logoutButton)
    {
        auto loginScene = LoginScene::createScene(0);
        Director::getInstance()->replaceScene(loginScene);
    }
    
}

