//
//  SettingsFriendshipsPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 04/09/2018.
//

#include "SettingsFriendshipsPage.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include "FriendRequestLayer.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsFriendshipsPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsFriendshipsPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText(StringMgr::getInstance()->getStringForKey(SETTINGS_PAGE_HEADING_FRIENDSHIPS));
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _friendRequestList = ui::ListView::create();
    _friendRequestList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _friendRequestList->setBounceEnabled(true);
    _friendRequestList->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height - 366));
    _friendRequestList->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _friendRequestList->setItemsMargin(50);
    _friendRequestList->setTopPadding(50);
    _friendRequestList->setBottomPadding(50);
    this->addChild(_friendRequestList);
    
    _pendingFRHttpRequest = API::GetPendingFriendRequests(this);
    _pendingFRHttpRequest->execute();
    
    Super::onEnter();
}

void SettingsFriendshipsPage::onExit()
{
    if(_pendingFRHttpRequest)
    {
        _pendingFRHttpRequest->clearDelegate();
    }
    
    Super::onExit();
}


void SettingsFriendshipsPage::addFriendRequestsToScrollView()
{
    for(int i = 0; i < ParentDataProvider::getInstance()->getNoOfPendingFriendRequest(); i++)
    {
        const std::string& senderName = ParentDataProvider::getInstance()->getPendingFriendRequestSenderName(i);
        const std::string& senderCode = "";
        const std::string& recipientName = ParentDataProvider::getInstance()->getPendingFriendRequestFriendName(i);
        const std::string& recipientCode = ParentDataProvider::getInstance()->getPendingFriendRequestInviteCode(i);
        const std::string& respondentId = ParentDataProvider::getInstance()->getPendingFriendRequestRespondentID(i);
        const std::string& requestId = ParentDataProvider::getInstance()->getPendingFriendRequestRequestID(i);
        
        FriendRequestLayer* friendRequest = FriendRequestLayer::create();
        friendRequest->setContentSize(Size(_friendRequestList->getContentSize().width, 500));
        friendRequest->setChildDetails(senderName, senderCode, recipientName, recipientCode, respondentId, requestId);
        _friendRequestList->pushBackCustomItem(friendRequest);
        
    }
}

//-----------DELETGATE FUNCTIONS-----------

void SettingsFriendshipsPage::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    ParentDataParser::getInstance()->parsePendingFriendRequests(body);
    addFriendRequestsToScrollView();
}

void SettingsFriendshipsPage::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsPendingFriendRequestsRefreshError(errorCode);
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}
NS_AZOOMEE_END



