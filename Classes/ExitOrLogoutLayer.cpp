#include "ExitOrLogoutLayer.h"
#include "LoginScene.h"
#include "Locale.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "ParentDataParser.h"
#include <AzoomeeCommon/Data/ParentDataProvider.h>
#include "ElectricDreamsTextStyles.h"
#include "MessageBox.h"
#include "ElectricDreamsDecoration.h"
#include "PaymentSingleton.h"
#include "IAPUpsaleLayer.h"
#include "cocos/ui/UIRichText.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>

#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

using namespace Azoomee;


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
    
    auto versionTitle = createLabelAppVerison(ConfigStorage::getInstance()->getVersionNumberToDisplay());
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
    
    if(PaymentSingleton::getInstance()->showIAPContent())
    {
        iapButton = ElectricDreamsButton::createButtonWithWidth("Start Trial",windowLayer->getContentSize().width/2);
        iapButton->setCenterPosition(Vec2(windowLayer->getContentSize().width /2, windowLayer->getContentSize().height*.6));
        iapButton->setDelegate(this);
        iapButton->setMixPanelButtonName("ExitorLogoutStartTrialButton");
        windowLayer->addChild(iapButton);
    }
    else if(ParentDataProvider::getInstance()->isPaidUser())
        addRichTextLabel("Premium Account");
    
    else if (ParentDataProvider::getInstance()->emailRequiresVerification())
    {
        Label* subTitleLabel = createLabelHeaderWhite("We need to verify your email address.\nPlease check your email or go to parent.azoomee.com for help.");
        subTitleLabel->setWidth(windowLayer->getContentSize().width - 50);
        subTitleLabel->setPosition(windowLayer->getContentSize().width/2,windowLayer->getContentSize().height*.6);
        windowLayer->addChild(subTitleLabel);
    }
    else
        addRichTextLabel("Free Account");
    
    // ------- LOG OUT BUTTON ----------
    
    logoutButton = ElectricDreamsButton::createSecondaryButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_LOG_OUT), windowLayer->getContentSize().width/2);
    logoutButton->setCenterPosition(Vec2(windowLayer->getContentSize().width /2, windowLayer->getContentSize().height*.3));
    logoutButton->setDelegate(this);
    logoutButton->setMixPanelButtonName("Log Out");
    windowLayer->addChild(logoutButton);
}

void ExitOrLogoutLayer::addRichTextLabel(std::string BOLDText)
{
    cocos2d::ui::RichText* richTextLabel = cocos2d::ui::RichText::create();
    richTextLabel->setAnchorPoint(Vec2(0.5,0.5));
    
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, "You have a ", FONT_REGULAR, 84));
    richTextLabel->pushBackElement(ui::RichElementText::create(0, Color3B::WHITE, 255, BOLDText, FONT_BOLD, 84));
    richTextLabel->setPosition(Vec2(windowLayer->getContentSize().width/2,windowLayer->getContentSize().height*.6));
    windowLayer->addChild(richTextLabel);
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
    {
        AnalyticsSingleton::getInstance()->displayIAPUpsaleEvent("Settings");
        IAPUpsaleLayer::create();
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
