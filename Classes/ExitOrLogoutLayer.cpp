#include "ExitOrLogoutLayer.h"
#include "LoginScene.h"
#include "StringStorage.h"
#include "ChildDataProvider.h"
#include "AudioMixer.h"
#include "MixPanelSingleton.h"
#include "ParentDataParser.h"

bool ExitOrLogoutLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createAndFadeInLayer();
    askForPin();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void ExitOrLogoutLayer::askForPin()
{
    AwaitingAdultPinLayer::create()->setDelegate(this);
}

void ExitOrLogoutLayer::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(0,0,0,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void ExitOrLogoutLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

void ExitOrLogoutLayer::addExitOrLogoutUIObjects()
{
    //-------- VERSION NUBMER ---------
    
    auto versionTitle = Label::createWithTTF(APP_VERSION_NUMBER, "fonts/azoomee.ttf", 60);
    versionTitle->setPosition(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.05);
    versionTitle->setColor(Color3B(28, 244, 244));
    backgroundLayer->addChild(versionTitle);
    
    // ------- CANCEL BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createCancelButton();
    cancelButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.3));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("ExitorLogoutCancelButton");
    backgroundLayer->addChild(cancelButton);
    
    // ------- LOG OUT BUTTON ----------
    
    logoutButton = ElectricDreamsButton::createButtonWithText(LOG_OUT_BUTTON_TEXT);
    logoutButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2, origin.y + visibleSize.height * 0.6));
    logoutButton->setDelegate(this);
    logoutButton->setMixPanelButtonName("Log Out");
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
    {
        AudioMixer::getInstance()->resumeBackgroundMusic();
        removeSelf();
    }
    else if(button == logoutButton)
    {
        MixPanelSingleton::getInstance()->mixPanel_logoutParent();
        ParentDataParser::getInstance()->logoutChild();
        
        UserDefault::getInstance()->getStringForKey("password", "");
        UserDefault::getInstance()->flush();
        
        AudioMixer::getInstance()->stopBackgroundMusic();
        
        auto loginScene = LoginScene::createScene(0);
        Director::getInstance()->replaceScene(loginScene);
    }
}

void ExitOrLogoutLayer::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
    removeSelf();
}

void ExitOrLogoutLayer::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    addExitOrLogoutUIObjects();
}
