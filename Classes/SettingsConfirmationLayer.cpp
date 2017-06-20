#include "SettingsConfirmationLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "ConfirmationControlLayer.h"

using namespace Azoomee;

#define TEST_NO_OF_CONFIRMATIONS 3
#define MARGIN 78

Layer* SettingsConfirmationLayer::createWithHeight(float setLayerHeight)
{
    //---QUESTION---- SHOULD WE GET AVAILABLE CHILDREN BEFORE BUILDING THIS - INCASE A NEW CHILD IS CREATED?
    
    
    auto layer = SettingsConfirmationLayer::create();
    layer->layerHeight = setLayerHeight;
    layer->setContentSize(Size(Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height));
    layer->contentWidth = Director::getInstance()->getVisibleSize().width - MARGIN*2;
    layer->addDetailsLabel();
    layer->addScrollView();
    
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

void SettingsConfirmationLayer::addDetailsLabel()
{
    std::string labelText = "Great news, some of your childrens's friend requests have been accepted. Before we can connect them we would need you to tap confirm.";
    
    
    if(TEST_NO_OF_CONFIRMATIONS == 0)
        labelText = "You're children don't have any friend requests to confirm at the moment.";
    
    detailsLabel = createLabelSettingsChat(labelText,Color3B::WHITE);
    detailsLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    detailsLabel->setAnchorPoint(Vec2(0,1));
    detailsLabel->setWidth(contentWidth);
    detailsLabel->setPosition(MARGIN,layerHeight - MARGIN);
    this->addChild(detailsLabel);
}

void SettingsConfirmationLayer::addScrollView()
{
    Size innerSize = Size(contentWidth,TEST_NO_OF_CONFIRMATIONS*182 + 4);
    
    float scrollViewHeight = layerHeight-MARGIN*3-detailsLabel->getContentSize().height;
    
    if(scrollViewHeight > innerSize.height)
        scrollViewHeight = innerSize.height;
    
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(contentWidth, scrollViewHeight));
    
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(true);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(innerSize);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    scrollView->setAnchorPoint(Vec2(0,1));
    scrollView->setPosition(Vec2(MARGIN,layerHeight-MARGIN*2-detailsLabel->getContentSize().height));
    
    this->addChild(scrollView);

    for(int i = 0; i < TEST_NO_OF_CONFIRMATIONS; i++)
    {
        auto confirmationLayer = ConfirmationControlLayer::createController(Size(contentWidth, 182), i);
        confirmationLayer->setPosition(0,i*182+2);
        scrollView->addChild(confirmationLayer,200);
    }
}
