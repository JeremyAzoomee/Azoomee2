#include "ModalMessages.h"
#include <ui/UIEditBox/UIEditBox.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon//Utils/LocaleManager.h>
#include "Style.h"

USING_NS_CC;

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

#define LOADING_Z_ORDER 5000

USING_NS_TZ

NS_AZ_BEGIN

const std::string ModalMessages::kLoadingLayerName = "loadingLayer";

static ModalMessages *_sharedModalMessages = NULL;

ModalMessages* ModalMessages::getInstance()
{
    if (! _sharedModalMessages)
    {
        _sharedModalMessages = new ModalMessages();
    }
    
    return _sharedModalMessages;
}

ModalMessages::~ModalMessages(void)
{
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
    loadingLayer->setName(kLoadingLayerName);
    Director::getInstance()->getRunningScene()->addChild(loadingLayer, LOADING_Z_ORDER);
    
    loadingLayer->runAction(FadeTo::create(0.5, 200));
}

void ModalMessages::removeLayer()
{
    if(Director::getInstance()->getRunningScene()->getChildByName(kLoadingLayerName))
    {
        Director::getInstance()->getRunningScene()->removeChild(loadingLayer);
    }
}

void ModalMessages::startLoading()
{
    if(Director::getInstance()->getRunningScene()->getChildByName(kLoadingLayerName)) return;
    
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
        if(CCRANDOM_0_1() < 0.5)
        {
            direction = -1;
        }
        
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
    if(Director::getInstance()->getRunningScene()->getChildByName(kLoadingLayerName))
    {
        return;
    }
    createAndFadeInLayer();
    
    auto savingLabel = Label::createWithTTF(_("Saving..."), Style::Font::Regular(), 128);
    savingLabel->setColor(Colours::Color_3B::white);
    savingLabel->setNormalizedPosition(Vec2(0.5,0.5));
    savingLabel->setAnchorPoint(Vec2(0.5,0.5));
    loadingLayer->addChild(savingLabel);
    
}

void ModalMessages::stopSaving()
{
    this->removeLayer();
}
  
NS_AZ_END

