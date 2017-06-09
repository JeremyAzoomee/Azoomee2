#include "SettingsKidsLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "ChildOomeeLayer.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>


#define FONT_REGULAR "fonts/Sofia Pro Soft Regular.otf"
#define FONT_BOLD "fonts/Sofia Pro Soft Bold.otf"

using namespace Azoomee;

Layer* SettingsKidsLayer::createWithHeight(float setLayerHeight)
{
    auto layer = SettingsKidsLayer::create();
    layer->layerHeight = setLayerHeight;
    
    return layer;
}

bool SettingsKidsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    this->setContentSize(Size(visibleSize.width,layerHeight));
    
    addExitOrLogoutUIObjects();
    return true;
}

//----------------Add UI Objects-------------

void SettingsKidsLayer::addExitOrLogoutUIObjects()
{
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(visibleSize.width * 0.8, 1200));
    scrollView->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(Size(ParentDataProvider::getInstance()->getAmountOfAvailableChildren()*800,1200));
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    scrollView->setAnchorPoint(Vec2(0.5,0));
    
    this->addChild(scrollView);
    

    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        std::string oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildren(i);
        int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
        
        auto childLayer = ChildOomeeLayer::create();
        childLayer->setPosition(i*800,0);
        scrollView->addChild(childLayer);
    }

}

