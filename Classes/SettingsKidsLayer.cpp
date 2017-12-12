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

Layer* SettingsKidsLayer::createWithSize(const cocos2d::Size &contentSize)
{
    auto layer = SettingsKidsLayer::create();
    layer->setContentSize(contentSize);
    layer->addTabsCoverLayer();
    layer->addScrollView();
    
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

void SettingsKidsLayer::addTabsCoverLayer()
{
    _tabsCoverLayer = LayerColor::create(Color4B(0,0,0,200), Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
    _tabsCoverLayer->setPosition((this->getContentSize().width - _tabsCoverLayer->getContentSize().width) / 2, 0);
    _tabsCoverLayer->setVisible(false);
    this->addChild(_tabsCoverLayer);
    
    addListenerToCoverLayer(_tabsCoverLayer);
}

void SettingsKidsLayer::addScrollView()
{
    Size innerSize = Size(ParentDataProvider::getInstance()->getAmountOfAvailableChildren() * 900 + 100, this->getContentSize().height);
    
    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    _scrollView->setBounceEnabled(false);
    _scrollView->setTouchEnabled(true);
    _scrollView->setInnerContainerSize(innerSize);
    _scrollView->setSwallowTouches(false);
    _scrollView->setScrollBarEnabled(true);
    _scrollView->setAnchorPoint(Vec2(0.5f, 0.5f));
    _scrollView->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
    _scrollView->setBackGroundColor(Color3B::ORANGE);
    
    this->addChild(_scrollView);
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        auto childLayer = KidsControlLayer::createController(this, i);
        childLayer->setPosition(100 + i * 900, _scrollView->getContentSize().height / 2 - childLayer->getContentSize().height / 2);
        childLayer->setTag(i);
        _scrollView->addChild(childLayer);
    }
}

void SettingsKidsLayer::addListenerToCoverLayer(Layer* listenerToLayer)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        //only swallow the touches if the tabsCoverLayer is visble on screen
        return _tabsCoverLayer->isVisible();
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), listenerToLayer);
}

//------------ACTIONS-----------------------------

void SettingsKidsLayer::selectChildForTextInput(int ChildNumber)
{
    _tabsCoverLayer->setVisible(true);

    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        _scrollView->getChildByTag(i)->setVisible(false);
    }
    
    _scrollView->getChildByTag(ChildNumber)->setVisible(true);
    
    _scrollView->stopAutoScroll();
    _scrollView->setTouchEnabled(false);
    _scrollView->setScrollBarEnabled(false);
    _scrollView->setInnerContainerPosition(Vec2(_scrollView->getContentSize().width/2 - ChildNumber *900-400,0));
}

void SettingsKidsLayer::selectChildForSharing(int ChildNumber)
{
    _scrollView->stopAutoScroll();
    _scrollView->setInnerContainerPosition(Vec2(_scrollView->getContentSize().width/2 - ChildNumber *900-400,0));
}

void SettingsKidsLayer::deleteChild(int ChildNumber)
{
    childNumberToDelete = ChildNumber;
    
    MessageBox::createWith(ERROR_CODE_DELETE_PROFILE, this);
}

void SettingsKidsLayer::scrollReset()
{
    _tabsCoverLayer->setVisible(false);
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        _scrollView->getChildByTag(i)->setVisible(true);
    }
    _scrollView->setTouchEnabled(true);
    _scrollView->setScrollBarEnabled(true);
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
        _scrollView->removeFromParent();
        addScrollView();
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
        _scrollView->removeFromParent();
        MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
    }
}

NS_AZOOMEE_END

