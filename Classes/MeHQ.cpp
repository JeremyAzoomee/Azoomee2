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
#include "HQDataProvider.h"
#include "HQScene2CarouselTitle.h"
#include "PrivacyAndTermsLayer.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

#include "MeHQProfileDetails.h"
#include "MeHQGallery.h"
#include "MeHQDownloads.h"
#include "MeHQFavourites.h"
#include "MeHQMessages.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string MeHQ::kProfileLayerName = "profile";
const std::string MeHQ::kGalleryLayerName = "gallery";
const std::string MeHQ::kMessagesLayerName = "messages";
const std::string MeHQ::kDownloadsLayerName = "downloads";
const std::string MeHQ::kFavoritesLayerName = "favourites";

std::string MeHQ::_previousLayer = kProfileLayerName;

bool MeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& contentSize = Director::getInstance()->getVisibleSize();
    
    this->setContentSize(contentSize);
    
    _contentListView = ui::ListView::create();
    _contentListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _contentListView->setBounceEnabled(true);
    _contentListView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    _contentListView->setItemsMargin(150.0f);
    _contentListView->setBottomPadding(100.0f);
    _contentListView->setContentSize(Size(contentSize.width, contentSize.height - ((ConfigStorage::getInstance()->isDeviceIphoneX() && contentSize.width < contentSize.height) ? 400 : 300)));
    _contentListView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(_contentListView);
    
    Sprite* verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
    verticalScrollGradient->setAnchorPoint(Vec2(0.5, 1.0));
    verticalScrollGradient->setScaleX(contentSize.width / verticalScrollGradient->getContentSize().width);
    verticalScrollGradient->setColor(Color3B::BLACK);
    verticalScrollGradient->setPosition(Vec2(contentSize.width / 2, _contentListView->getContentSize().height));
    this->addChild(verticalScrollGradient);
    
    int indexNum = 0;
    
    auto profileLayout = MeHQProfileDetails::create();
    profileLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    profileLayout->setName(kProfileLayerName);
    _contentListView->pushBackCustomItem(profileLayout);
    _sectionIndexMap[kProfileLayerName] = indexNum++;
    
    auto messageList = MeHQMessages::create();
    messageList->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    messageList->setRefreshCallback([this](){
        this->refreshMessagesLayout();
    });
    messageList->setName(kMessagesLayerName);
    _contentListView->addChild(messageList);
    _sectionIndexMap[kMessagesLayerName] = indexNum++;
    
    auto galleryLayout = MeHQGallery::create();
    galleryLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    galleryLayout->setName(kGalleryLayerName);
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

void MeHQ::refreshFavouritesLayout()
{
    float scrollPercent = _contentListView->getScrolledPercentVertical();
    _contentListView->removeItem(_sectionIndexMap[kFavoritesLayerName]);
    auto favouriteLayout = MeHQFavourites::create();
    favouriteLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    favouriteLayout->setRefreshCallback([this](){
        this->refreshFavouritesLayout();
    });
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
    bool expanded = false;
    if(galleryLayer)
    {
        expanded = galleryLayer->_expanded;
    }
    _contentListView->removeItem(_sectionIndexMap[kGalleryLayerName]);
    auto galleryLayout = MeHQGallery::create();
    galleryLayout->_expanded = expanded;
    galleryLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _contentListView->insertCustomItem(galleryLayout, _sectionIndexMap[kGalleryLayerName]);
    
    _contentListView->forceDoLayout();
    _contentListView->scrollToPercentVertical(scrollPercent, 0, true);
}

float MeHQ::getScrollPercent()
{
    return _contentListView->getScrolledPercentVertical();
}

NS_AZOOMEE_END
