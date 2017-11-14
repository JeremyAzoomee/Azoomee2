#include "SettingsConfirmationLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "ConfirmationControlLayer.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

NS_AZOOMEE_BEGIN

#define MARGIN 78
#define CONFIRMATION_HEIGHT 182

Layer* SettingsConfirmationLayer::createWithHeight(float setLayerHeight)
{
    auto layer = SettingsConfirmationLayer::create();
    layer->layerHeight = setLayerHeight;
    layer->setContentSize(Size(Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height));
    layer->contentWidth = Director::getInstance()->getVisibleSize().width - MARGIN*2;
    layer->setName("ConfirmationLayer");
    
    ModalMessages::getInstance()->startLoading();
    
    return layer;
}

bool SettingsConfirmationLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _pendingFRHttpRequest = API::GetPendingFriendRequests(this);
    _pendingFRHttpRequest->execute();
    
    return true;
}

//----------------Add UI Objects-------------

void SettingsConfirmationLayer::confirmationDetailsReceived()
{
    ModalMessages::getInstance()->stopLoading();
    addDetailsLabel();
    addScrollView();
}


void SettingsConfirmationLayer::addDetailsLabel()
{
    std::string labelText = "You have new friendships to approve!";
    
    
    if(ParentDataProvider::getInstance()->getNoOfPendingFriendRequest() == 0)
        labelText = "There are no new friendships to approve right now.";
    
    detailsLabel = createLabelSettingsChat(labelText,Color3B::BLACK);
    detailsLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    detailsLabel->setAnchorPoint(Vec2(0,1));
    detailsLabel->setWidth(contentWidth);
    detailsLabel->setPosition(MARGIN,layerHeight - MARGIN);
    this->addChild(detailsLabel);
}

void SettingsConfirmationLayer::addScrollView()
{
    Size innerSize = Size(contentWidth,ParentDataProvider::getInstance()->getNoOfPendingFriendRequest()*CONFIRMATION_HEIGHT + 4);
    
    float scrollViewHeight = layerHeight-MARGIN*3-detailsLabel->getContentSize().height;
    
    if(scrollViewHeight > innerSize.height)
        scrollViewHeight = innerSize.height;
    
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(contentWidth, scrollViewHeight));
    
    scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(false);
    scrollView->setTouchEnabled(true);
    scrollView->setInnerContainerSize(innerSize);
    scrollView->setSwallowTouches(false);
    scrollView->setScrollBarEnabled(true);
    scrollView->setAnchorPoint(Vec2(0,1));
    scrollView->setPosition(Vec2(MARGIN,layerHeight-MARGIN*2-detailsLabel->getContentSize().height));
    
    this->addChild(scrollView);

    for(int i = 0; i < ParentDataProvider::getInstance()->getNoOfPendingFriendRequest(); i++)
    {
        auto confirmationLayer = ConfirmationControlLayer::createController(Size(contentWidth, CONFIRMATION_HEIGHT), i);
        confirmationLayer->setPosition(0,i*CONFIRMATION_HEIGHT+2);
        scrollView->addChild(confirmationLayer,CONFIRMATION_IDLE_Z);
    }
}

//-----------DELETGATE FUNCTIONS-----------

void SettingsConfirmationLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    ParentDataParser::getInstance()->parsePendingFriendRequests(body);
    confirmationDetailsReceived();
}

void SettingsConfirmationLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsPendingFriendRequestsRefreshError(errorCode);
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}

SettingsConfirmationLayer::~SettingsConfirmationLayer()
{
    if(_pendingFRHttpRequest)
    {
        _pendingFRHttpRequest->clearDelegate();
    }
}

NS_AZOOMEE_END
