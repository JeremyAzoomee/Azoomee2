#include "SettingsControlLayer.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Json.h>
#include "AccountDetailsLayer.h"
#include "SettingsKidsLayer.h"
#include "SettingsConfirmationLayer.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

#define LINE_WIDTH 4
#define TAB_SPACING 50
#define SELECTED_TAB_Z 111
#define IDLE_TAB_Z 100
#define CURRENT_LAYER_Z 120

NS_AZOOMEE_BEGIN

bool SettingsControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createSettingsLayer();
    AwaitingAdultPinLayer::create()->setDelegate(this);
    
    return true;
}

void SettingsControlLayer::createSettingsLayer()
{
    backgroundLayer = LayerColor::create(Color4B::BLACK,origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->setName("SettingsControlLayer");
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToLayer(backgroundLayer);
}

void SettingsControlLayer::addListenerToLayer(Layer* layerToAdd)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), layerToAdd);
}

//---------------------- Create Settings Tabs and Objects -----------------------------

void SettingsControlLayer::createSettingsController()
{
    createCancelButton();
    createLine();
    createTabs();
    createConfirmationNotification();
    checkForConfirmationNotifications();
    
    selectNewTab(SettingsKidsLayer::createWithHeight(linePositionY-LINE_WIDTH/2), childrenButton);
}

void SettingsControlLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setCenterPosition(Vec2(origin.x + visibleSize.width - cancelButton->getContentSize().width, origin.y + visibleSize.height - cancelButton->getContentSize().height));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("CancelSettingsButton");
    backgroundLayer->addChild(cancelButton);
}

void SettingsControlLayer::createLine()
{
    linePositionY = visibleSize.height-cancelButton->getContentSize().height*2;
    
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->setLineWidth(LINE_WIDTH);
    newDrawNode->drawLine(Vec2(0, origin.y+linePositionY), Vec2(visibleSize.width, origin.y+linePositionY), Color4F(28.0/255, 244.0/255, 244.0/255, 1.0));
    backgroundLayer->addChild(newDrawNode,110);
}

void SettingsControlLayer::createTabs()
{
    childrenButton = ElectricDreamsButton::createTabButton("Your Kids");
    childrenButton->setPosition(TAB_SPACING*2,origin.y+linePositionY-LINE_WIDTH);
    childrenButton->setDelegate(this);
    childrenButton->setMixPanelButtonName("SettingsTab-YourKids");
    backgroundLayer->addChild(childrenButton,IDLE_TAB_Z);
    
    confirmationButton = ElectricDreamsButton::createTabButton("Friendships");
    confirmationButton->setPosition(childrenButton->getPositionX()+childrenButton->getContentSize().width/2+ TAB_SPACING+confirmationButton->getContentSize().width/2,origin.y+linePositionY-LINE_WIDTH);
    confirmationButton->setDelegate(this);
    confirmationButton->setMixPanelButtonName("SettingsTab-TheirFriends");
    backgroundLayer->addChild(confirmationButton,IDLE_TAB_Z);
    
    accountButton = ElectricDreamsButton::createTabButton("Your Account");
    accountButton->setPosition(confirmationButton->getPositionX()+confirmationButton->getContentSize().width/2+TAB_SPACING+accountButton->getContentSize().width/2,origin.y+linePositionY-LINE_WIDTH);
    accountButton->setDelegate(this);
    confirmationButton->setMixPanelButtonName("SettingsTab-Account");
    backgroundLayer->addChild(accountButton,SELECTED_TAB_Z);
}

void SettingsControlLayer::createConfirmationNotification()
{
    confirmationNotification = Sprite::create("res/navigation/chatAlert.png");
    confirmationNotification->setPosition(confirmationButton->getPositionX()+confirmationButton->getContentSize().width-LINE_WIDTH,confirmationButton->getPositionY()+confirmationButton->getContentSize().height-LINE_WIDTH);
    confirmationNotification->setOpacity(0);
    backgroundLayer->addChild(confirmationNotification,SELECTED_TAB_Z);
}

void SettingsControlLayer::checkForConfirmationNotifications()
{
    HttpRequestCreator *request = API::getPendingFriendRequests(this);
    request->execute();
}

//---------------------- Actions -----------------

void SettingsControlLayer::removeSelf()
{
    AudioMixer::getInstance()->resumeBackgroundMusic();
    this->removeChild(backgroundLayer);
    this->removeFromParent();
}

void SettingsControlLayer::selectNewTab(Layer* newCurrentLayer, ElectricDreamsButton* buttonToBringForward)
{
    if(currentTabLayer)
        currentTabLayer->removeFromParent();
    childrenButton->setLocalZOrder(IDLE_TAB_Z);
    confirmationButton->setLocalZOrder(IDLE_TAB_Z);
    accountButton->setLocalZOrder(IDLE_TAB_Z);
    
    buttonToBringForward->setLocalZOrder(SELECTED_TAB_Z);
    
    currentTabLayer = newCurrentLayer;
    currentTabLayer->setPosition(origin.x,origin.y);
    backgroundLayer->addChild(currentTabLayer,CURRENT_LAYER_Z);
}

//----------------------- Delegate Functions ----------------------------

void SettingsControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
        removeSelf();
    else if(button == childrenButton)
        selectNewTab(SettingsKidsLayer::createWithHeight(linePositionY-LINE_WIDTH/2), childrenButton);
    else if(button == confirmationButton)
    {
        confirmationNotification->setOpacity(0);
        selectNewTab(SettingsConfirmationLayer::createWithHeight(linePositionY-LINE_WIDTH/2), confirmationButton);
    }
    else if(button == accountButton)
        selectNewTab(AccountDetailsLayer::createWithHeight(linePositionY-LINE_WIDTH/2), accountButton);
}

void SettingsControlLayer::AdultPinCancelled(AwaitingAdultPinLayer* layer)
{
    removeSelf();
}

void SettingsControlLayer::AdultPinAccepted(AwaitingAdultPinLayer* layer)
{
    createSettingsController();
}

void SettingsControlLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    rapidjson::Document pendingFriendRequestData;
    pendingFriendRequestData.Parse(body.c_str());
    
    if(pendingFriendRequestData.Size() >0)
    {
        AnalyticsSingleton::getInstance()->settingsConfirmationTabNotificationShown();
        confirmationNotification->setOpacity(255);
    }
}

void SettingsControlLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsConfirmationTabNotificationError(errorCode);
}

NS_AZOOMEE_END
