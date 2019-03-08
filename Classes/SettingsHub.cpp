//
//  SettingsHub.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/08/2018.
//

#include "SettingsHub.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "SceneManagerScene.h"
#include "SettingsSupportPage.h"
#include "SettingsOnlineSafetyPage.h"
#include "SettingsYourAccountPage.h"
#include "SettingsFriendshipsPage.h"
#include "SettingsKidsPage.h"
#include "SettingsLanguagePage.h"
#include "HQHistoryManager.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsHub::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
	
	_prevLangCode = StringMgr::getInstance()->getLanguageID();
	
    LayerColor* bgColour = LayerColor::create(Color4B::WHITE, visibleSize.width, visibleSize.height);
    this->addChild(bgColour);
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setContentSize(visibleSize);
    _contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Style::Color::greyBlue);
    _contentLayout->setBackGroundColorOpacity(50);
    _contentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    this->addChild(_contentLayout);
    
    bool isIphoneX = ConfigStorage::getInstance()->isDeviceIphoneX();
    
    _titleLayout = ui::Layout::create();
    _titleLayout->setContentSize(Size(visibleSize.width, isIphoneX ? 250 : 150));
    _titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(Style::Color::skyBlue);
    _contentLayout->addChild(_titleLayout);
    
    _titleBarButton = ui::Button::create("res/settings/exit_button.png");
    _titleBarButton->setNormalizedPosition(isIphoneX ? Vec2(0,0.33f) : Vec2::ANCHOR_MIDDLE_LEFT);
    _titleBarButton->setAnchorPoint(Vec2(0.0f,0.5f));
	_titleBarButton->setContentSize(Size(150, 150));
	_titleBarButton->ignoreContentAdaptWithSize(false);
    _titleLayout->addChild(_titleBarButton);
    _titleBarButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_inHub)
            {
				HQHistoryManager::getInstance()->setReturnedFromForcedOrientation(true);
				switch(_origin)
				{
					case SettingsOrigin::HQ:
						Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
						break;
					case SettingsOrigin::CHILD_SELECT:
						Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChildSelector));
						break;
					case SettingsOrigin::CHAT:
						Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
						break;
				}
            }
            else
            {
                _activeSettingsPageHolder->setVisible(false);
                _navigationLayout->setVisible(true);
                _titleText->setString(_("Settings"));
                _titleBarButton->loadTextureNormal("res/settings/exit_button.png");
				if(_prevLangCode != StringMgr::getInstance()->getLanguageID())
				{
					_prevLangCode = StringMgr::getInstance()->getLanguageID();
					this->updateTextForNewLanguage();
				}
                _inHub = true;
            }
        }
    });
    
    _titleText = ui::Text::create(_("Settings"), Style::Font::Medium(), 91);
    _titleText->setNormalizedPosition(isIphoneX ? Vec2(0.5f,0.25f) : Vec2::ANCHOR_MIDDLE);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleText->setTextColor(Color4B::WHITE);
    _titleLayout->addChild(_titleText);
    
    _mainBodyLayout = ui::Layout::create();
    _mainBodyLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleLayout->getContentSize().height));
    _contentLayout->addChild(_mainBodyLayout);
    
    _navigationLayout = ui::ListView::create();
	_navigationLayout->setDirection(ui::ScrollView::Direction::VERTICAL);
	_navigationLayout->setBounceEnabled(true);
	_navigationLayout->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
	_navigationLayout->setItemsMargin(10.0f);
	_navigationLayout->setBottomPadding(10.0f);
	_navigationLayout->setTopPadding(10.0f);
	_navigationLayout->setSwallowTouches(true);
    _navigationLayout->setContentSize(_mainBodyLayout->getContentSize());
    _mainBodyLayout->addChild(_navigationLayout);
    
    _activeSettingsPageHolder = ui::Layout::create();
    _activeSettingsPageHolder->setContentSize(_mainBodyLayout->getContentSize());
    _activeSettingsPageHolder->setVisible(false);
    _mainBodyLayout->addChild(_activeSettingsPageHolder);
	
	const float numButtonsOnScreen = 6.5;
	
	const float buttonHeight = _navigationLayout->getContentSize().height * (1.0f / numButtonsOnScreen) - 10;
	
	_languageButton = SettingsNavigationButton::create();
	_languageButton->setContentSize(Size(visibleSize.width, buttonHeight));
	_languageButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
	_languageButton->setIconFilename(StringUtils::format("res/settings/flag_%s.png",StringMgr::getInstance()->getLanguageID().c_str()));
	_languageButton->setTitleText(_("Language"));
	_languageButton->setSubTitleText(_("Change your language selection"));
	_languageButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->changeToPage(SettingsPages::LANGUAGE);
		}
	});
	_languageButton->setTouchEnabled(true);
	_navigationLayout->addChild(_languageButton);
	
    _kidsButton = SettingsNavigationButton::create();
    _kidsButton->setContentSize(Size(visibleSize.width, buttonHeight));
    _kidsButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _kidsButton->setIconFilename("res/settings/your_kids_icon_2.png");
    _kidsButton->setTitleText(_("Your Kids"));
    _kidsButton->setSubTitleText(_("Manage your kids’ profiles and add friends using Kid Codes"));
    _kidsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::KIDS);
        }
    });
    _kidsButton->setTouchEnabled(true);
    _navigationLayout->addChild(_kidsButton);
    
    _friendshipsButton = SettingsNavigationButton::create();
    _friendshipsButton->setContentSize(Size(visibleSize.width, buttonHeight));
    _friendshipsButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _friendshipsButton->setIconFilename("res/settings/friendships_icon_3.png");
    _friendshipsButton->setTitleText(_("Friendships"));
    _friendshipsButton->setSubTitleText(_("Accept or reject new friendship requests"));
    _friendshipsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::FRIENDSHIPS);
        }
    });
    _friendshipsButton->setTouchEnabled(true);
    _navigationLayout->addChild(_friendshipsButton);
	
	_parentInboxButton = SettingsNavigationButton::create();
	_parentInboxButton->setContentSize(Size(visibleSize.width, buttonHeight));
	_parentInboxButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
	_parentInboxButton->setIconFilename("res/settings/friendships_icon_3.png");
	_parentInboxButton->setTitleText(_("Parent Inbox"));
	_parentInboxButton->setSubTitleText(_("Receive and send messages to your kids"));
	_parentInboxButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(ParentDataProvider::getInstance()->isPaidUser())
			{
				HttpRequestCreator* request = API::RefreshParentCookiesRequest(this);
				request->execute();
			}
		}
	});
	_parentInboxButton->setTouchEnabled(true);
	_navigationLayout->addChild(_parentInboxButton);
	
    _yourAccountButton = SettingsNavigationButton::create();
    _yourAccountButton->setContentSize(Size(visibleSize.width, buttonHeight));
    _yourAccountButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _yourAccountButton->setIconFilename("res/settings/your_account_icon_4.png");
    _yourAccountButton->setTitleText(_("Your Account"));
    _yourAccountButton->setSubTitleText(_("Update your details and manage your subscription"));
    _yourAccountButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::ACCOUNT);
        }
    });
    _yourAccountButton->setTouchEnabled(true);
    _navigationLayout->addChild(_yourAccountButton);
    
    _onlineSafetyButton = SettingsNavigationButton::create();
    _onlineSafetyButton->setContentSize(Size(visibleSize.width, buttonHeight));
    _onlineSafetyButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _onlineSafetyButton->setIconFilename("res/settings/online_safety_icon_4.png");
    _onlineSafetyButton->setTitleText(_("Online Safety"));
    _onlineSafetyButton->setSubTitleText(_("Tips and tricks to help your family stay safe online"));
    _onlineSafetyButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::ONLINE_SAFETY);
        }
    });
    _onlineSafetyButton->setTouchEnabled(true);
    _navigationLayout->addChild(_onlineSafetyButton);
    
    _supportButton = SettingsNavigationButton::create();
    _supportButton->setContentSize(Size(visibleSize.width, buttonHeight));
    _supportButton->setLayoutParameter(CreateTopLinearLayoutParam(ui::Margin(0,0,0,10)));
    _supportButton->setIconFilename("res/settings/support_icon_4.png");
    _supportButton->setTitleText(_("Support"));
    _supportButton->setSubTitleText(_("Need some help? View our support page or contact us directly"));
    _supportButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->changeToPage(SettingsPages::SUPPORT);
        }
    });
    _supportButton->setTouchEnabled(true);
    _navigationLayout->addChild(_supportButton);
    
    
    return true;
}

void SettingsHub::onEnter()
{
	if(_origin != SettingsOrigin::CHAT || ChildDataProvider::getInstance()->isChildLoggedIn())
	{
		RequestAdultPinLayer* pinLayer = RequestAdultPinLayer::create();
		pinLayer->setDelegate(this);
	}
	else // parent returning from chat
	{
		_origin = SettingsOrigin::CHILD_SELECT;
		ModalMessages::getInstance()->startLoading();
		HttpRequestCreator* request = API::getParentDetailsRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), this);
		request->execute();
	}
    Super::onEnter();
}

void SettingsHub::onSizeChanged()
{
    
}

void SettingsHub::setOrigin(SettingsOrigin origin)
{
	_origin = origin;
}

void SettingsHub::changeToPage(SettingsPages page)
{
    _activeSettingsPageHolder->removeAllChildren();
    switch(page)
    {
		case SettingsPages::LANGUAGE:
		{
			auto page = SettingsLanguagePage::create();
			page->setContentSize(_activeSettingsPageHolder->getContentSize());
			_activeSettingsPageHolder->addChild(page);
			_titleText->setString(_("Language"));
			break;
		}
		case SettingsPages::KIDS:
        {
            auto page = SettingsKidsPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString(_("Your Kids"));
            break;
        }
        case SettingsPages::FRIENDSHIPS:
        {
            auto page = SettingsFriendshipsPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString(_("Friendships"));
            break;
        }
        case SettingsPages::ACCOUNT:
        {
            auto page = SettingsYourAccountPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString(_("Your Account"));
            break;
        }
        case SettingsPages::ONLINE_SAFETY:
        {
            auto page = SettingsOnlineSafetyPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString(_("Online Safety"));
            break;
        }
        case SettingsPages::SUPPORT:
        {
            auto page = SettingsSupportPage::create();
            page->setContentSize(_activeSettingsPageHolder->getContentSize());
            _activeSettingsPageHolder->addChild(page);
            _titleText->setString(_("Support"));
            break;
        }
    }
    _activeSettingsPageHolder->setVisible(true);
    _navigationLayout->setVisible(false);
    _titleBarButton->loadTextureNormal("res/settings/toggle_switch_white.png");
    _inHub = false;
}

void SettingsHub::updateTextForNewLanguage()
{
	_titleText->setString(_("Settings"));
	_titleText->setFontName(Style::Font::Medium());
	
	_languageButton->setIconFilename(StringUtils::format("res/settings/flag_%s.png",StringMgr::getInstance()->getLanguageID().c_str()));
	_languageButton->setTitleText(_("Language"));
	_languageButton->setSubTitleText(_("Change your language selection"));
	
	_kidsButton->setTitleText(_("Your Kids"));
	_kidsButton->setSubTitleText(_("Manage your kids’ profiles and add friends using Kid Codes"));
	
	_friendshipsButton->setTitleText(_("Friendships"));
	_friendshipsButton->setSubTitleText(_("Accept or reject new friendship requests"));
	
	_yourAccountButton->setTitleText(_("Your Account"));
	_yourAccountButton->setSubTitleText(_("Update your details and manage your subscription"));
	
	_onlineSafetyButton->setTitleText(_("Online Safety"));
	_onlineSafetyButton->setSubTitleText(_("Tips and tricks to help your family stay safe online"));
	
	_supportButton->setTitleText(_("Support"));
	_supportButton->setSubTitleText(_("Need some help? View our support page or contact us directly"));
}

// delegate functions

void SettingsHub::AdultPinCancelled(RequestAdultPinLayer* layer)
{
	HQHistoryManager::getInstance()->setReturnedFromForcedOrientation(true);
	switch(_origin)
	{
		case SettingsOrigin::HQ:
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
			break;
		case SettingsOrigin::CHILD_SELECT:
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChildSelector));
			break;
		case SettingsOrigin::CHAT:
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
			break;
	}
}
void SettingsHub::AdultPinAccepted(RequestAdultPinLayer* layer)
{
	layer->removeFromParent();
	
	ModalMessages::getInstance()->startLoading();
	HttpRequestCreator* request = API::getParentDetailsRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), this);
	request->execute();
	
}

void SettingsHub::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	ModalMessages::getInstance()->stopLoading();
	if(requestTag == API::TagGetParentDetails)
	{
		ParentDataParser::getInstance()->parseParentDetails(body);
	}
	else if(requestTag == API::TagCookieRefresh)
	{
		ParentDataParser::getInstance()->parseParentSessionData(body);
		const std::string& userId = ParentDataProvider::getInstance()->getLoggedInParentId();
		const std::string& sessionId = ParentDataProvider::getInstance()->getLoggedInParentCdnSessionId();
		
		HttpRequestCreator* request = API::GetGordenRequest(userId, sessionId, this);
		request->execute();
		return;
	}
	else if(requestTag == API::TagGetGorden)
	{
		ChildDataParser::getInstance()->setChildLoggedIn(false);// make sure we log out child if entering parent chat
		if(CookieDataParser::getInstance()->parseDownloadCookies(headers))
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
		}
	}
}

void SettingsHub::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
}

NS_AZOOMEE_END
