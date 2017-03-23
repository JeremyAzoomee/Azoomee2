#include "ModalMessages.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "LoginScene.h"
#include "OnboardingScene.h"
#include "BaseScene.h"
#include "HQHistoryManager.h"
#include "AnalyticsSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MixPanelNotifications_ios.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MixPanelNotifications_android.h"
#endif

USING_NS_CC;

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

static ModalMessages *_sharedModalMessages = NULL;

ModalMessages* ModalMessages::getInstance()
{
    if (! _sharedModalMessages)
    {
        _sharedModalMessages = new ModalMessages();
        _sharedModalMessages->init();
    }
    
    return _sharedModalMessages;
}

ModalMessages::~ModalMessages(void)
{
}

bool ModalMessages::init(void)
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void ModalMessages::createAndFadeInLayer()
{
    loadingLayer = LayerColor::create(Color4B(0,0,0,150), visibleSize.width, visibleSize.height);
    loadingLayer->setPosition(origin.x, origin.y);
    loadingLayer->setOpacity(0);
    Director::getInstance()->getRunningScene()->addChild(loadingLayer);
    
    addListenerToBackgroundLayer();
    
    loadingLayer->runAction(FadeTo::create(0.5, 200));
}

void ModalMessages::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), loadingLayer);
}

void ModalMessages::removeLayer()
{
    if(loadingLayer) //This might be called when loading is not active, so better to check first
    {
        Director::getInstance()->getRunningScene()->removeChild(loadingLayer);
    }
}

void ModalMessages::startLoading()
{
    createAndFadeInLayer();
    
    //ADDING SOME TEMPORARY CIRCLES AS LOADING, NOT FINAL YET
    
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        loadingCircle->setOpacity(0);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6 + i * 0.2);
        
        loadingLayer->addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5) direction = -1;
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
        loadingCircle->runAction(FadeTo::create(0.5, 255));
    }
}

void ModalMessages::stopLoading()
{
    this->removeLayer();
}

//-----------------MIXPANEL NOTIFICATIONS--------------------------

void ModalMessages::showMixpanelNotification()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    showNotification_ios();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    showNotification_android();
    
#endif
}

void ModalMessages::showMixpanelNotificationWithID(int notificationID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    showNotificationWithID_ios(notificationID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    showNotificationWithID_android(notificationID);
    
#endif
}

