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
#include <AzoomeeCommon/Strings.h>

NS_AZOOMEE_BEGIN

#define MARGIN 78
#define CONFIRMATION_HEIGHT 182

Layer* SettingsConfirmationLayer::createWithSize(const cocos2d::Size &contentSize)
{
    auto layer = SettingsConfirmationLayer::create();
    layer->setContentSize(contentSize);
    layer->contentWidth = layer->getContentSize().width - MARGIN * 2;
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
    std::string labelText = StringMgr::getInstance()->getStringForKey(SETTINGS_FRIENDSHIPS_TO_APPROVE);
    
    
    if(ParentDataProvider::getInstance()->getNoOfPendingFriendRequest() == 0)
        labelText = StringMgr::getInstance()->getStringForKey(SETTINGS_NO_FRIENDSHIPS_TO_APPROVE);;
    
    detailsLabel = createLabelSettingsChat(labelText,Color3B::BLACK);
    detailsLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    detailsLabel->setAnchorPoint(Vec2(0,1));
    detailsLabel->setWidth(contentWidth);
    detailsLabel->setPosition(MARGIN, this->getContentSize().height - MARGIN);
    this->addChild(detailsLabel);
}

void SettingsConfirmationLayer::addScrollView()
{
    Size innerSize = Size(contentWidth,ParentDataProvider::getInstance()->getNoOfPendingFriendRequest()*CONFIRMATION_HEIGHT + 4);
    
    float scrollViewHeight = this->getContentSize().height - MARGIN * 3 - detailsLabel->getContentSize().height;
    
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
    scrollView->setPosition(Vec2(MARGIN, this->getContentSize().height - MARGIN * 2 - detailsLabel->getContentSize().height));
    
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
