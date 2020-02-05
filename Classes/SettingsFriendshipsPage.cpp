//
//  SettingsFriendshipsPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 04/09/2018.
//

#include "SettingsFriendshipsPage.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "FriendRequestLayer.h"
#include "PopupMessageBox.h"
#include "ErrorCodes.h"

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
    _headerBanner->setText(_("Parents must approve new friendships before any messages can be sent. You can accept or reject friendship requests here."));
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
    
    _pendingFRHttpRequest = API::GetPendingFriendRequests(UserAccountManager::getInstance()->getLoggedInParentId(), this);
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
    for(int i = 0; i < UserAccountManager::getInstance()->getNoOfPendingFriendRequest(); i++)
    {
		FriendRequestRef fr = UserAccountManager::getInstance()->getPendingFriendRequest(i);
		if(fr)
		{
			FriendRequestLayer* friendRequest = FriendRequestLayer::create();
			friendRequest->setContentSize(Size(_friendRequestList->getContentSize().width, 500));
			friendRequest->setChildDetails(fr);
			_friendRequestList->pushBackCustomItem(friendRequest);
		}
        
    }
}

//-----------DELETGATE FUNCTIONS-----------

void SettingsFriendshipsPage::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    UserAccountManager::getInstance()->parsePendingFriendRequests(body);
    addFriendRequestsToScrollView();
}

void SettingsFriendshipsPage::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsPendingFriendRequestsRefreshError(errorCode);
    ModalMessages::getInstance()->stopLoading();
    const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(errorCode);
        
    PopupMessageBox* messageBox = PopupMessageBox::create();
    messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
    messageBox->setBody(errorMessageText.at(ERROR_BODY));
    messageBox->setButtonText(_("Back"));
    messageBox->setButtonColour(Style::Color::darkIndigo);
    messageBox->setPatternColour(Style::Color::azure);
    messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
        pSender->removeFromParent();
    });
    this->addChild(messageBox, 1);
}
NS_AZOOMEE_END



