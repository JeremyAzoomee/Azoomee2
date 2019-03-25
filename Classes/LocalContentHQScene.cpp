//
//  LocalContentHQScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/01/2019.
//

#include "LocalContentHQScene.h"
#include "MeHQRecentlyPlayed.h"
#include "MeHQFavourites.h"
#include "MeHQDownloads.h"
#include "RecentlyPlayedManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>


using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool LocalContentHQScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_type = HQSceneType::LOCAL_CONTENT_HQ;
	
	return true;
}

void LocalContentHQScene::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
	_contentListView = ui::ListView::create();
	_contentListView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_contentListView->setBounceEnabled(true);
	_contentListView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
	_contentListView->setItemsMargin(150.0f);
	_contentListView->setTopPadding(100.0f);
	_contentListView->setBottomPadding(100.0f);
	_contentListView->setContentSize(Size(contentSize.width, contentSize.height - ((ConfigStorage::getInstance()->isDeviceIphoneX() && contentSize.width < contentSize.height) ? 600 : 500) - (_showingMessagingLayer ? 350 : 0)));
	_contentListView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_contentListView->setPosition(Vec2(0,300 + (_showingMessagingLayer ? 350 : 0)));
	_contentListView->setSwallowTouches(true);
	this->addChild(_contentListView);
	
	Sprite* verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
	verticalScrollGradient->setAnchorPoint(Vec2(0.5, 0.9));
	verticalScrollGradient->setScaleX(contentSize.width / verticalScrollGradient->getContentSize().width);
	verticalScrollGradient->setColor(Color3B::BLACK);
	verticalScrollGradient->setPosition(Vec2(contentSize.width / 2, _contentListView->getContentSize().height + 300));
	this->addChild(verticalScrollGradient);
	
	if(RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kMeHQName).size() > 0)
	{
		auto recentlyPlayed = MeHQRecentlyPlayed::create();
		recentlyPlayed->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
		_contentListView->pushBackCustomItem(recentlyPlayed);
	}
	
	auto favouriteLayout = MeHQFavourites::create();
	favouriteLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	favouriteLayout->setRefreshCallback([this](){
		
	});
	_contentListView->pushBackCustomItem(favouriteLayout);
	
	auto downloadsLayout = MeHQDownloads::create();
	downloadsLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	_contentListView->pushBackCustomItem(downloadsLayout);
	
	Super::onEnter();
}

void LocalContentHQScene::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
