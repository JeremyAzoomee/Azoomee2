//
//  NavigationScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/01/2019.
//

#include "NavigationScene.h"
#include "SceneManagerScene.h"
#include "HQScene2.h"
#include "HQHistoryManager.h"
#include "DynamicNodeHandler.h"
#include "CoinDisplay.h"
#include "SettingsButton.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool NavigationScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void NavigationScene::onEnter()
{
	HQHistoryManager::getInstance()->addHQToHistoryManager("Navigation");
	
	_gamesNavButton = createNavButton("Games");
	_gamesNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kGameHQName);
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	_gamesNavButton->setNormalizedPosition(Vec2(0.25,0.8));
	this->addChild(_gamesNavButton);
	
	_videosNavButton = createNavButton("Videos");
	_videosNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kVideoHQName);
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	_videosNavButton->setNormalizedPosition(Vec2(0.5,0.85));
	this->addChild(_videosNavButton);
	
	_chatNavButton = createNavButton("Chat");
	_chatNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
			{
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
			}
			else
			{
				AnalyticsSingleton::getInstance()->registerCTASource("lockedHQ","",ConfigStorage::kChatHQName);
				IAPEntryContext context = IAPEntryContext::LOCKED_CHAT;
#ifndef ALLOW_UNPAID_SIGNUP
				DynamicNodeHandler::getInstance()->startIAPFlow(context);
#else
				DynamicNodeHandler::getInstance()->startSignupFlow();
#endif
			}
		}
	});
	_chatNavButton->setNormalizedPosition(Vec2(0.75,0.8));
	this->addChild(_chatNavButton);
	
	_shopNavButton = createNavButton("Shop");
	_shopNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			
		}
	});
	_shopNavButton->setNormalizedPosition(Vec2(0.25,0.55));
	this->addChild(_shopNavButton);
	
	_galleryNavButton = createNavButton("Gallery");
	_galleryNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kArtAppHQName);
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	_galleryNavButton->setNormalizedPosition(Vec2(0.5,0.6));
	this->addChild(_galleryNavButton);
	
	_oomeeMakerNavButton = createNavButton("Oomee Maker");
	_oomeeMakerNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(OomeeMakerEntryPointScene));
		}
	});
	_oomeeMakerNavButton->setNormalizedPosition(Vec2(0.75,0.55));
	this->addChild(_oomeeMakerNavButton);
	
	_favouritesNavButton = createNavButton("Favourites");
	_favouritesNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kMeHQName);
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
		}
	});
	_favouritesNavButton->setNormalizedPosition(Vec2(0.25,0.3));
	this->addChild(_favouritesNavButton);
	
	_questsNavButton = createNavButton("Quests");
	_questsNavButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Button::TouchEventType::ENDED)
		{
			
		}
	});
	_questsNavButton->setNormalizedPosition(Vec2(0.75,0.3));
	this->addChild(_questsNavButton);
	
	CoinDisplay* coinDisplay = CoinDisplay::create();
	coinDisplay->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	coinDisplay->setAnchorPoint(Vec2(1.2,1.5));
	this->addChild(coinDisplay);
	
	SettingsButton* settingsButton = SettingsButton::create();
	settingsButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	settingsButton->setAnchorPoint(Vec2(-0.25,1.25));
	this->addChild(settingsButton);
	
	if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
	{
		ui::Button* childSelectButton = ui::Button::create("res/navigation/menu_childSelect.png");
		childSelectButton->setAnchorPoint(Vec2(-0.25,1.25));
		childSelectButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
		childSelectButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Button::TouchEventType::ENDED)
			{
				ChildDataParser::getInstance()->setChildLoggedIn(false);
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
			}
		});
		this->addChild(childSelectButton);
	}
	
	Super::onEnter();
}

ui::Button* NavigationScene::createNavButton(const std::string& buttonText)
{
	ui::Button* button = ui::Button::create("res/decoration/whiteWindowScale9.png");
	button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	float size = MIN(this->getContentSize().width * 0.2f,this->getContentSize().height * 0.2f);
	button->setContentSize(Size(size, size));
	button->setScale9Enabled(true);
	
	Label* text = Label::createWithTTF(buttonText, Style::Font::Regular(), 50);
	text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	text->setTextColor(Color4B::BLACK);
	text->setOverflow(Label::Overflow::SHRINK);
	text->setHorizontalAlignment(TextHAlignment::CENTER);
	text->setVerticalAlignment(TextVAlignment::CENTER);
	text->setDimensions(button->getContentSize().width * 0.8f, button->getContentSize().height * 0.8f);
	
	button->addChild(text);
	return button;
}

NS_AZOOMEE_END
