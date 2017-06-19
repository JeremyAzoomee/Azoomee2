#include "SettingsConfirmationLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

using namespace Azoomee;

Layer* SettingsConfirmationLayer::createWithHeight(float setLayerHeight)
{
    //---QUESTION---- SHOULD WE GET AVAILABLE CHILDREN BEFORE BUILDING THIS - INCASE A NEW CHILD IS CREATED?
    
    
    auto layer = SettingsConfirmationLayer::create();
    layer->layerHeight = setLayerHeight;
    layer->setContentSize(Size(Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height));
    layer->addExitOrLogoutUIObjects();
    
    return layer;
}

bool SettingsConfirmationLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void SettingsConfirmationLayer::addExitOrLogoutUIObjects()
{
    Size innerSize = Size(ParentDataProvider::getInstance()->getAmountOfAvailableChildren()*900,1200);
    
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(this->getContentSize().width * 0.9, 1200));
    scrollView->setPosition(Vec2(this->getContentSize().width/2,layerHeight/2));
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(innerSize);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    scrollView->setAnchorPoint(Vec2(0.5,0.5));
    
    this->addChild(scrollView,50);

    for(int i = 0; i < 3; i++)
    {

    }
}
