#include "SettingsKidsLayer.h"
#include "KidsControlLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

using namespace Azoomee;

Layer* SettingsKidsLayer::createWithHeight(float setLayerHeight)
{
    //---QUESTION---- SHOULD WE GET AVAILABLE CHILDREN BEFORE BUILDING THIS - INCASE A NEW CHILD IS CREATED?
    
    auto layer = SettingsKidsLayer::create();
    layer->layerHeight = setLayerHeight;
    layer->setContentSize(Size(Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height));
    layer->addUIObjects();
    
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

void SettingsKidsLayer::addUIObjects()
{
    Size innerSize = Size(ParentDataProvider::getInstance()->getAmountOfAvailableChildren()*900,1200);
    
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(this->getContentSize().width, 1200));
    scrollView->setPosition(Vec2(this->getContentSize().width/2,layerHeight/2));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setBounceEnabled(false);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(innerSize);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    scrollView->setAnchorPoint(Vec2(0.5,0.5));
    
    this->addChild(scrollView,50);
    
    createBlackCoverLayer(innerSize);

    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        auto childLayer = KidsControlLayer::createController(this, i);
        childLayer->setPosition(i*900,0);
        scrollView->addChild(childLayer,IDLE_KID_LAYER_Z_ORDER);
    }
}

void SettingsKidsLayer::createBlackCoverLayer(Size innerSize)
{
    kidsCoverLayer = LayerColor::create(Color4B::BLACK,innerSize.width, innerSize.height);
    scrollView->addChild(kidsCoverLayer,IDLE_COVER_LAYER_Z_ORDER);
    addListenerToCoverLayer(kidsCoverLayer);
}

void SettingsKidsLayer::addListenerToCoverLayer(Layer* listenerToLayer)
{
    swallowTouches = false;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return swallowTouches;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), listenerToLayer);
}

//------------ACTIONS-----------------------------

void SettingsKidsLayer::selectChild(int ChildNumber)
{
    addTabsCoverLayer();
    swallowTouches = true;
    
    kidsCoverLayer->setLocalZOrder(SELECTED_COVER_LAYER_Z_ORDER);
    scrollView->stopAutoScroll();
    scrollView->setTouchEnabled(false);
    scrollView->setScrollBarEnabled(false);
    scrollView->setInnerContainerPosition(Vec2(scrollView->getContentSize().width/2 - ChildNumber *900-400,0));
}

void SettingsKidsLayer::scrollReset()
{
    tabsCoverLayer->removeFromParent();
    swallowTouches = false;
    
    kidsCoverLayer->setLocalZOrder(IDLE_COVER_LAYER_Z_ORDER);
    scrollView->setTouchEnabled(true);
    scrollView->setScrollBarEnabled(true);
}

void SettingsKidsLayer::addTabsCoverLayer()
{
    tabsCoverLayer = LayerColor::create(Color4B(0,0,0,200),this->getContentSize().width, Director::getInstance()->getVisibleSize().height-layerHeight+10);
    tabsCoverLayer->setPosition(0,layerHeight-10);
    this->addChild(tabsCoverLayer,40);
    
    addListenerToCoverLayer(tabsCoverLayer);
}


