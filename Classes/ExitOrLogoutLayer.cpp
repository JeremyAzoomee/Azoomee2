#include "ExitOrLogoutLayer.h"
#include "LoginScene.h"
#include "StringMgr.h"
#include "ChildDataProvider.h"
#include "AudioMixer.h"
#include "AnalyticsSingleton.h"
#include "ParentDataParser.h"
#include "ParentDataProvider.h"
#include "ElectricDreamsTextStyles.h"
#include "MessageBox.h"
#include "ElectricDreamsDecoration.h"
#include "PaymentSingleton.h"
#include "IAPUpsaleLayer.h"

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
    backgroundLayer = LayerColor::create(Color4B(15,14,7,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
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
    addSideWiresToScreen(this, 0, 2);
    
    windowLayer = createWindowLayer(1100);
    float windowLayerBottomPadding = (visibleSize.height - windowLayer->getContentSize().height) * .66;
    windowLayer->setPosition(visibleSize.width/2- windowLayer->getContentSize().width/2,origin.y + windowLayerBottomPadding);
    this->addChild(windowLayer);
    
    //-------- VERSION NUBMER ---------
    
    auto versionTitle = createLabelAppVerison(APP_VERSION_NUMBER);
    versionTitle->setPosition(windowLayer->getContentSize().width/2,versionTitle->getContentSize().height * 1.5);
    windowLayer->addChild(versionTitle);
    
    //-------- USERNAME---------------
    
    Label* usernameLabel = createUserNameLabelWithWidth(windowLayer->getContentSize().width - 50);
    usernameLabel->setPosition(windowLayer->getContentSize().width/2,windowLayer->getContentSize().height*.85);
    windowLayer->addChild(usernameLabel);

    //-------- CLOSE BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("ExitorLogoutCancelButton");
    windowLayer->addChild(cancelButton);
    
    // ------- START IAP OR STATUS ----------
    
    //if(PaymentSingleton::getInstance()->showIAPContent())
    if(true)
    {
        iapButton = ElectricDreamsButton::createButtonWithWidth("Start Trial",windowLayer->getContentSize().width/2);
        iapButton->setCenterPosition(Vec2(windowLayer->getContentSize().width /2, windowLayer->getContentSize().height*.6));
        iapButton->setDelegate(this);
        iapButton->setMixPanelButtonName("ExitorLogoutStartTrialButton");
        windowLayer->addChild(iapButton);
    }
    else if(ParentDataProvider::getInstance()->isPaidUser())
    {
        Label* subTitleLabel = createLabelHeaderWhite("PREMIUM ACCOUNT");
        subTitleLabel->setPosition(windowLayer->getContentSize().width/2,windowLayer->getContentSize().height*.6);
        windowLayer->addChild(subTitleLabel);
    }
    else if (ParentDataProvider::getInstance()->emailRequiresVerification())
    {
        Label* subTitleLabel = createLabelHeaderWhite("Email address requires verification!\nCheckout parent.azoomee.com for help.");
        subTitleLabel->setPosition(windowLayer->getContentSize().width/2,windowLayer->getContentSize().height*.6);
        windowLayer->addChild(subTitleLabel);
    }
    else
    {
        Label* subTitleLabel = createLabelHeaderWhite("FREE ACCOUNT");
        subTitleLabel->setPosition(windowLayer->getContentSize().width/2,windowLayer->getContentSize().height*.6);
        windowLayer->addChild(subTitleLabel);
    }
    
    // ------- LOG OUT BUTTON ----------
    
    logoutButton = ElectricDreamsButton::createSecondaryButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_OUT), windowLayer->getContentSize().width/2);
    logoutButton->setCenterPosition(Vec2(windowLayer->getContentSize().width /2, windowLayer->getContentSize().height*.3));
    logoutButton->setDelegate(this);
    logoutButton->setMixPanelButtonName("Log Out");
    windowLayer->addChild(logoutButton);
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
        AnalyticsSingleton::getInstance()->logoutParentEvent();
        ParentDataParser::getInstance()->logoutChild();
        
        UserDefault::getInstance()->getStringForKey("password", "");
        UserDefault::getInstance()->flush();
        
        AudioMixer::getInstance()->stopBackgroundMusic();
        
        auto loginScene = LoginScene::createScene(0);
        Director::getInstance()->replaceScene(loginScene);
    }
    else if(button == iapButton)
        IAPUpsaleLayer::create();
}

void ExitOrLogoutLayer::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
    removeSelf();
}

void ExitOrLogoutLayer::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    addExitOrLogoutUIObjects();
}
