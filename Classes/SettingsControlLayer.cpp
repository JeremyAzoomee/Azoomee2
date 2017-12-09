#include "SettingsControlLayer.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Data/Json.h>
#include "AccountDetailsLayer.h"
#include "SettingsKidsLayer.h"
#include "SettingsConfirmationLayer.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include "OnlineSafetyDetailsLayer.h"
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

#define LINE_WIDTH 4
#define TAB_SPACING 50
#define SELECTED_TAB_Z 111
#define IDLE_TAB_Z 100
#define CURRENT_LAYER_Z 120

NS_AZOOMEE_BEGIN

Layer* SettingsControlLayer::createFromChat()
{
    auto layer = SettingsControlLayer::create();
    layer->returnToChatScene = true;
    
    return layer;
}

bool SettingsControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        _visibleSize.width -= 200;
        _origin.x += 100;
    }
    
    this->setPosition(_origin);
    this->setContentSize(_visibleSize);
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createSettingsLayer();
    AwaitingAdultPinLayer::create()->setDelegate(this);
    
    return true;
}

void SettingsControlLayer::createSettingsLayer()
{
    //we need an additional white background layer to cover the top and right edges of the screen if we are on iPhone 10
    //we cannot extend the background, because all additional layers are added to that. This is bad practice...
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        LayerColor *additionalBackground = LayerColor::create(Color4B::WHITE, this->getContentSize().width + 200, this->getContentSize().height);
        additionalBackground->setPosition(-100, 0);
        this->addChild(additionalBackground);
    }
    
    backgroundLayer = LayerColor::create(Color4B::WHITE, this->getContentSize().width, this->getContentSize().height);    
    
    this->setName("SettingsControlLayer");
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    Layer* test = createPixelsPatternAndGradient();
    test->setPosition(0, 0);
    
    if(ConfigStorage::getInstance()->isDeviceIphoneX())
    {
        test->setPosition(-100, 0);
    }
    
    backgroundLayer->addChild(test);
    
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
    
    selectNewTab(SettingsKidsLayer::createWithSize(this->getContentSize()), childrenButton);
}

void SettingsControlLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloseButtonGreen();
    cancelButton->setCenterPosition(Vec2(this->getContentSize().width - cancelButton->getContentSize().width, this->getContentSize().height - cancelButton->getContentSize().height));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("CancelSettingsButton");
    backgroundLayer->addChild(cancelButton);
}

void SettingsControlLayer::createLine()
{
    linePositionY = this->getContentSize().height - cancelButton->getContentSize().height*2;
    
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->setLineWidth(LINE_WIDTH);
    newDrawNode->drawLine(Vec2(0, linePositionY), Vec2(this->getContentSize().width, linePositionY), Style::Color_4F::greenish);
    backgroundLayer->addChild(newDrawNode,110);
}

void SettingsControlLayer::createTabs()
{
    childrenButton = ElectricDreamsButton::createTabButton("Your Kids");
    childrenButton->setPosition(TAB_SPACING * 2, linePositionY - LINE_WIDTH);
    childrenButton->setDelegate(this);
    childrenButton->setMixPanelButtonName("SettingsTab-YourKids");
    backgroundLayer->addChild(childrenButton,IDLE_TAB_Z);
    
    confirmationButton = ElectricDreamsButton::createTabButton("Friendships");
    confirmationButton->setPosition(childrenButton->getPositionX() + childrenButton->getContentSize().width / 2 + TAB_SPACING + confirmationButton->getContentSize().width / 2, linePositionY-LINE_WIDTH);
    confirmationButton->setDelegate(this);
    confirmationButton->setMixPanelButtonName("SettingsTab-TheirFriends");
    backgroundLayer->addChild(confirmationButton, IDLE_TAB_Z);
    
    onlineSafetyButton = ElectricDreamsButton::createTabButton("Online Safety");
    onlineSafetyButton->setPosition(confirmationButton->getPositionX() + confirmationButton->getContentSize().width / 2 + TAB_SPACING + onlineSafetyButton->getContentSize().width / 2, linePositionY - LINE_WIDTH);
    onlineSafetyButton->setDelegate(this);
    onlineSafetyButton->setMixPanelButtonName("SettingsTab-OnlineSafety");
    backgroundLayer->addChild(onlineSafetyButton, IDLE_TAB_Z);
    
    accountButton = ElectricDreamsButton::createTabButton("Your Account");
    accountButton->setPosition(onlineSafetyButton->getPositionX() + onlineSafetyButton->getContentSize().width / 2 + TAB_SPACING + accountButton->getContentSize().width / 2 , linePositionY-LINE_WIDTH);
    accountButton->setDelegate(this);
    accountButton->setMixPanelButtonName("SettingsTab-Account");
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
    _pendingFRHttpRequest = API::GetPendingFriendRequests(this);
    _pendingFRHttpRequest->execute();
}

//---------------------- Actions -----------------

void SettingsControlLayer::removeSelf()
{
    if(returnToChatScene)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
    }
    else if(FlowDataSingleton::getInstance()->getChildDeletedFlag())
    {
        FlowDataSingleton::getInstance()->resetChildDeletedFlag();
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    }
    else
    {
        AudioMixer::getInstance()->resumeBackgroundMusic();
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}

void SettingsControlLayer::selectNewTab(Layer* newCurrentLayer, ElectricDreamsButton* buttonToBringForward)
{
    if(currentTabLayer)
        currentTabLayer->removeFromParent();
    childrenButton->setLocalZOrder(IDLE_TAB_Z);
    confirmationButton->setLocalZOrder(IDLE_TAB_Z);
    accountButton->setLocalZOrder(IDLE_TAB_Z);
    onlineSafetyButton->setLocalZOrder(IDLE_TAB_Z);
    
    buttonToBringForward->setLocalZOrder(SELECTED_TAB_Z);
    
    currentTabLayer = newCurrentLayer;
    currentTabLayer->setPosition(0, 0);
    backgroundLayer->addChild(currentTabLayer,CURRENT_LAYER_Z);
}

//----------------------- Delegate Functions ----------------------------

void SettingsControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
        removeSelf();
    else if(button == childrenButton)
        selectNewTab(SettingsKidsLayer::createWithSize(this->getContentSize()), childrenButton);
    else if(button == confirmationButton)
    {
        confirmationNotification->setOpacity(0);
        selectNewTab(SettingsConfirmationLayer::createWithSize(Size(this->getContentSize().width, linePositionY-LINE_WIDTH/2)), confirmationButton);
    }
    else if(button == accountButton)
        selectNewTab(AccountDetailsLayer::createWithSize(Size(this->getContentSize().width, linePositionY - LINE_WIDTH / 2)), accountButton);
    else if(button == onlineSafetyButton)
        selectNewTab(OnlineSafetyDetailsLayer::createWithSize(Size(this->getContentSize().width, linePositionY - LINE_WIDTH / 2)), onlineSafetyButton);
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

SettingsControlLayer::~SettingsControlLayer()
{
    if(_pendingFRHttpRequest)
    {
        _pendingFRHttpRequest->clearDelegate();
    }
}

NS_AZOOMEE_END
