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

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsFriendshipsPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    
    _pendingFRHttpRequest = API::GetPendingFriendRequests(this);
    _pendingFRHttpRequest->execute();
    
    return true;
}

void SettingsFriendshipsPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText("Learn more about managing your account… adipiscing elit. consectetur adipiscing elit.");
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _friendRequestList = ui::ListView::create();
    _friendRequestList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _friendRequestList->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height - 416));
    _friendRequestList->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,50,0)));
    _friendRequestList->setItemsMargin(50);
    this->addChild(_friendRequestList);
    
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


//-----------DELETGATE FUNCTIONS-----------

void SettingsFriendshipsPage::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    ParentDataParser::getInstance()->parsePendingFriendRequests(body);
}

void SettingsFriendshipsPage::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsPendingFriendRequestsRefreshError(errorCode);
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}
NS_AZOOMEE_END



