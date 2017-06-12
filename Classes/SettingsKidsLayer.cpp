#include "SettingsKidsLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "SettingsKidsChildLayer.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>



using namespace Azoomee;

Layer* SettingsKidsLayer::createWithHeight(float setLayerHeight)
{
    //---QUESTION---- SHOULD WE GET AVAILABLE CHILDREN BEFORE BUILDING THIS - INCASE A NEW CHILD IS CREATED?
    
    
    auto layer = SettingsKidsLayer::create();
    layer->layerHeight = setLayerHeight;
    layer->setContentSize(Size(Director::getInstance()->getVisibleSize().width,setLayerHeight));
    layer->addExitOrLogoutUIObjects();
    
    return layer;
}

bool SettingsKidsLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void SettingsKidsLayer::addExitOrLogoutUIObjects()
{
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(this->getContentSize().width * 0.8, 1200));
    scrollView->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(Size(ParentDataProvider::getInstance()->getAmountOfAvailableChildren()*800,1200));
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    scrollView->setAnchorPoint(Vec2(0.5,0.5));
    
    this->addChild(scrollView);
    

    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        std::string oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildren(i);
        int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
        
        auto childLayer = SettingsKidsChildLayer::createWithChildDetails(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(i),oomeeNr);
        childLayer->setPosition(i*900,0);
        scrollView->addChild(childLayer);
    }

}

