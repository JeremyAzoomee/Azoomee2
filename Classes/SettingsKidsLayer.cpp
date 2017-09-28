#include "SettingsKidsLayer.h"
#include "KidsControlLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "FlowDataSingleton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Layer* SettingsKidsLayer::createWithHeight(float setLayerHeight)
{
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
    
    scrollView = cocos2d::ui::ScrollView::create();
    scrollView->setContentSize(Size(this->getContentSize().width, 1275));
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
        childLayer->setPosition(i*900,70);
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

void SettingsKidsLayer::selectChildForTextInput(int ChildNumber)
{
    addTabsCoverLayer();
    swallowTouches = true;
    
    kidsCoverLayer->setLocalZOrder(SELECTED_COVER_LAYER_Z_ORDER);
    scrollView->stopAutoScroll();
    scrollView->setTouchEnabled(false);
    scrollView->setScrollBarEnabled(false);
    scrollView->setInnerContainerPosition(Vec2(scrollView->getContentSize().width/2 - ChildNumber *900-400,0));
}

void SettingsKidsLayer::selectChildForSharing(int ChildNumber)
{
    scrollView->stopAutoScroll();
    scrollView->setInnerContainerPosition(Vec2(scrollView->getContentSize().width/2 - ChildNumber *900-400,0));
}

void SettingsKidsLayer::deleteChild(int ChildNumber)
{
    childNumberToDelete = ChildNumber;
    
    MessageBox::createWith(ERROR_CODE_DELETE_PROFILE, this);
}

void SettingsKidsLayer::scrollReset()
{
    removeTabsCoverLayer();
    swallowTouches = false;
    
    kidsCoverLayer->setLocalZOrder(IDLE_COVER_LAYER_Z_ORDER);
    scrollView->setTouchEnabled(true);
    scrollView->setScrollBarEnabled(true);
}

void SettingsKidsLayer::addTabsCoverLayer()
{
    removeTabsCoverLayer();
    
    tabsCoverLayer = LayerColor::create(Color4B(0,0,0,200),this->getContentSize().width, Director::getInstance()->getVisibleSize().height-layerHeight+10);
    tabsCoverLayer->setPosition(0,layerHeight-10);
    tabsCoverLayer->setName("tabsCoverLayer");
    
    this->addChild(tabsCoverLayer,40);
    
    addListenerToCoverLayer(tabsCoverLayer);

}

void SettingsKidsLayer::removeTabsCoverLayer()
{
    if(this->getChildByName("tabsCoverLayer"))
        this->removeChildByName("tabsCoverLayer");
}

//--------------- DELEGATE FUNCTIONS-------------------

void SettingsKidsLayer::MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle)
{
    if(buttonTitle == MessageBox::kDelete)
    {
        ModalMessages::getInstance()->startLoading();
        
        HttpRequestCreator* request = API::DeleteChild(ParentDataProvider::getInstance()->getIDForAvailableChildren(childNumberToDelete),
                                                       ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumberToDelete),
                                                       ParentDataProvider::getInstance()->getSexForAnAvailableChildren(childNumberToDelete),
                                                       this);
        request->execute();
    }
}

void SettingsKidsLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagDeleteChild)
    {
        FlowDataSingleton::getInstance()->setChildDeletedFlag();
        HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
        request->execute();
        
    }
    else if(requestTag == API::TagGetAvailableChildren)
    {
        ParentDataParser::getInstance()->parseAvailableChildren(body);
        scrollView->removeFromParent();
        addUIObjects();
        ModalMessages::getInstance()->stopLoading();
    }
}

void SettingsKidsLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    
    if(requestTag == API::TagDeleteChild)
    {
        MessageBox::createWith(ERROR_CODE_CANNOT_DELETE_PROFILE, nullptr);
    }
    else if(requestTag == API::TagGetAvailableChildren)
    {
        scrollView->removeFromParent();
        MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
    }
}

NS_AZOOMEE_END

