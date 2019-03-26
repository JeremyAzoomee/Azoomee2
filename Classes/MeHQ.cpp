//
//  MeHQ.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/03/2018.
//

#include "MeHQ.h"
#include "HQScene2.h"
#include "HQSceneElement.h"
#include "SceneManagerScene.h"
#include "RecentlyPlayedManager.h"
#include "HQScene2ElementPositioner.h"
#include "FavouritesManager.h"
#include "RecentlyPlayedManager.h"
#include "HQDataProvider.h"
#include "HQScene2CarouselTitle.h"
#include "PrivacyAndTermsLayer.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

#include "MeHQProfileDetails.h"
#include "MeHQGallery.h"
#include "MeHQDownloads.h"
#include "MeHQFavourites.h"
#include "MeHQMessages.h"
#include "MeHQRecentlyPlayed.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string MeHQ::kProfileLayerName = "profile";
const std::string MeHQ::kGalleryLayerName = "gallery";
const std::string MeHQ::kMessagesLayerName = "messages";
const std::string MeHQ::kDownloadsLayerName = "downloads";
const std::string MeHQ::kFavoritesLayerName = "favourites";
const std::string MeHQ::kRecentlyPlayedLayerName = "recentlyPlayed";

std::string MeHQ::_previousLayer = kProfileLayerName;

bool MeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
	
	_contentNode = Node::create();
	_contentNode->setContentSize(this->getContentSize());
	this->addChild(_contentNode);
    
	buildListView();
	
	_settingsButton = SettingsButton::create();
	_settingsButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_settingsButton->setAnchorPoint(Vec2(-0.25,1.25));
	this->addChild(_settingsButton,1);
	
    return true;
}

void MeHQ::onEnter()
{
    Super::onEnter();
    if(_previousLayer != kProfileLayerName)
    {
        Director::getInstance()->getScheduler()->schedule([=](float deltat)
        {
            _contentListView->forceDoLayout();
            _contentListView->scrollToItem(_sectionIndexMap[_previousLayer], Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
        }, this, 0, 0, 0, false, "scroll");
    }
}

void MeHQ::onExit()
{
    _previousLayer = _contentListView->getCenterItemInCurrentView()->getName();
    cocos2d::log("Center Layer: %s",_previousLayer.c_str());
    Super::onExit();
}

void MeHQ::onSizeChanged()
{
	Super::onSizeChanged();
	float scrollPercent = _contentListView->getScrolledPercentVertical();
	if(isnan(scrollPercent))
	{
		scrollPercent = 0.1f;
	}
	
	_contentNode->removeAllChildren();
	
	buildListView();
	Director::getInstance()->getScheduler()->schedule([=](float deltat)
	{
		_contentListView->scrollToPercentVertical(scrollPercent, 0, true);
	}, this, 0, 0, 0, false, "scroll");
}

void MeHQ::buildListView()
{
	const Size& contentSize = this->getContentSize();
	
	_contentListView = ui::ListView::create();
	_contentListView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_contentListView->setBounceEnabled(true);
	_contentListView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
	_contentListView->setItemsMargin(150.0f);
	_contentListView->setBottomPadding(100.0f);
	_contentListView->setContentSize(Size(contentSize.width, contentSize.height - ((ConfigStorage::getInstance()->isDeviceIphoneX() && contentSize.width < contentSize.height) ? 600 : 500) - (_showingMessagingLayer ? 350 : 0)));
	_contentListView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_contentListView->setPosition(Vec2(0,300 + (_showingMessagingLayer ? 350 : 0)));
	_contentListView->setSwallowTouches(true);
	_contentNode->addChild(_contentListView);
	
	Sprite* verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
	verticalScrollGradient->setAnchorPoint(Vec2(0.5, 0.9));
	verticalScrollGradient->setScaleX(contentSize.width / verticalScrollGradient->getContentSize().width);
	verticalScrollGradient->setColor(Color3B::BLACK);
	verticalScrollGradient->setPosition(Vec2(contentSize.width / 2, _contentListView->getPositionY() + _contentListView->getContentSize().height));
	_contentNode->addChild(verticalScrollGradient);
	
	int indexNum = 0;
	
	auto profileLayout = MeHQProfileDetails::create();
	profileLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	profileLayout->setName(kProfileLayerName);
	_contentListView->pushBackCustomItem(profileLayout);
	_sectionIndexMap[kProfileLayerName] = indexNum++;
	
	if(RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kMeHQName).size() > 0)
	{
		auto recentlyPlayed = MeHQRecentlyPlayed::create();
		recentlyPlayed->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
		recentlyPlayed->setName(kRecentlyPlayedLayerName);
		_contentListView->pushBackCustomItem(recentlyPlayed);
		_sectionIndexMap[kRecentlyPlayedLayerName] = indexNum++;
	}
	
	if(!ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
	{
		auto messageList = MeHQMessages::create();
		messageList->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
		messageList->setRefreshCallback([this](){
			this->refreshMessagesLayout();
		});
		messageList->setName(kMessagesLayerName);
		_contentListView->addChild(messageList);
		_sectionIndexMap[kMessagesLayerName] = indexNum++;
	}
	
	auto galleryLayout = MeHQGallery::create();
	galleryLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	galleryLayout->setName(kGalleryLayerName);
	galleryLayout->setRefreshCallback([this](){
		this->refreshGalleryLayout();
	});
	_contentListView->pushBackCustomItem(galleryLayout);
	_sectionIndexMap[kGalleryLayerName] = indexNum++;
	
	auto favouriteLayout = MeHQFavourites::create();
	favouriteLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	favouriteLayout->setRefreshCallback([this](){
		this->refreshFavouritesLayout();
	});
	favouriteLayout->setName(kFavoritesLayerName);
	_contentListView->pushBackCustomItem(favouriteLayout);
	_sectionIndexMap[kFavoritesLayerName] = indexNum++;
	
	auto downloadsLayout = MeHQDownloads::create();
	downloadsLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	downloadsLayout->setName(kDownloadsLayerName);
	_contentListView->pushBackCustomItem(downloadsLayout);
	_sectionIndexMap[kDownloadsLayerName] = indexNum++;
}

void MeHQ::refreshFavouritesLayout()
{
    float scrollPercent = _contentListView->getScrolledPercentVertical();
    MeHQFavourites* favsLayer = dynamic_cast<MeHQFavourites*>(_contentListView->getItem(_sectionIndexMap[kFavoritesLayerName]));
    bool editEnabled = favsLayer->getEditEnabled();
    _contentListView->removeItem(_sectionIndexMap[kFavoritesLayerName]);
    auto favouriteLayout = MeHQFavourites::create();
    favouriteLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	favouriteLayout->setName(kFavoritesLayerName);
    favouriteLayout->setRefreshCallback([this](){
        this->refreshFavouritesLayout();
    });
	favouriteLayout->setEditEnabled(editEnabled);
    _contentListView->insertCustomItem(favouriteLayout, _sectionIndexMap[kFavoritesLayerName]);
    
    _contentListView->forceDoLayout();
    _contentListView->scrollToPercentVertical(scrollPercent, 0, true);
}

void MeHQ::refreshMessagesLayout()
{
    _contentListView->forceDoLayout();
}

void MeHQ::refreshGalleryLayout()
{
    float scrollPercent = _contentListView->getScrolledPercentVertical();
    MeHQGallery* galleryLayer = dynamic_cast<MeHQGallery*>(_contentListView->getItem(_sectionIndexMap[kGalleryLayerName]));
	bool editEnabled = galleryLayer->getEditEnabled();
    bool expanded = galleryLayer->_expanded;
    _contentListView->removeItem(_sectionIndexMap[kGalleryLayerName]);
    auto galleryLayout = MeHQGallery::create();
    galleryLayout->_expanded = expanded;
    galleryLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	galleryLayout->setName(kGalleryLayerName);
	galleryLayout->setEditEnabled(editEnabled);
	galleryLayout->setRefreshCallback([this](){
		this->refreshGalleryLayout();
	});
    _contentListView->insertCustomItem(galleryLayout, _sectionIndexMap[kGalleryLayerName]);
    
    _contentListView->forceDoLayout();
    _contentListView->scrollToPercentVertical(scrollPercent, 0, true);
}

float MeHQ::getScrollPercent()
{
    return _contentListView->getScrolledPercentVertical();
}

NS_AZOOMEE_END
