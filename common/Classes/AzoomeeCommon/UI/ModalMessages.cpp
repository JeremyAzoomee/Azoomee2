#include "ModalMessages.h"
#include <ui/UIEditBox/UIEditBox.h>
#include "../Analytics/AnalyticsSingleton.h"
#include "../Mixpanel/Notifications.h"
#include "Style.h"
#include "Scene.h"
#include "../Strings.h"

USING_NS_CC;

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

#define LOADING_Z_ORDER 5000

NS_AZOOMEE_BEGIN

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
    
    return true;
}

void ModalMessages::createAndFadeInLayer()
{
    loadingLayer = ui::Layout::create();
    loadingLayer->setSizeType(ui::Layout::SizeType::PERCENT);
    loadingLayer->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    loadingLayer->setBackGroundColor(Color3B::BLACK);
    loadingLayer->setSizePercent(Vec2(1.0f, 1.0f));
    loadingLayer->setSwallowTouches(true);
    loadingLayer->setTouchEnabled(true);
    loadingLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    loadingLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    loadingLayer->setOpacity(0);
    loadingLayer->setName("loadingLayer");
    Director::getInstance()->getRunningScene()->addChild(loadingLayer, LOADING_Z_ORDER);
    
    loadingLayer->runAction(FadeTo::create(0.5, 200));
}

void ModalMessages::removeLayer()
{
    if(Director::getInstance()->getRunningScene()->getChildByName("loadingLayer"))
    {
        Director::getInstance()->getRunningScene()->removeChild(loadingLayer);
    }
}

void ModalMessages::startLoading()
{
    if(Director::getInstance()->getRunningScene()->getChildByName("loadingLayer")) return;
    
    createAndFadeInLayer();
    
    //ADDING SOME TEMPORARY CIRCLES AS LOADING, NOT FINAL YET
    
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setNormalizedPosition(Vec2(0.5,0.5));
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

void ModalMessages::startSaving()
{
    if(Director::getInstance()->getRunningScene()->getChildByName("loadingLayer")) return;
    
    createAndFadeInLayer();
    
    auto savingLabel = Label::createWithTTF(_("Saving..."), Style::Font::Regular(), 128);
    savingLabel->setColor(Style::Color::white);
    savingLabel->setNormalizedPosition(Vec2(0.5,0.5));
    savingLabel->setAnchorPoint(Vec2(0.5,0.5));
    loadingLayer->addChild(savingLabel);
    
}

void ModalMessages::stopSaving()
{
    this->removeLayer();
}

//-----------------MIXPANEL NOTIFICATIONS--------------------------

void ModalMessages::showMixpanelNotification()
{
    showMixPanelNotification();
}

void ModalMessages::showMixpanelNotificationWithID(int notificationID)
{
    showMixPanelNotificationWithID(notificationID);
}
  
NS_AZOOMEE_END

