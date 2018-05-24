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

bool MeHQ::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Director::getInstance()->getVisibleSize());
    
    _contentListView = ui::ListView::create();
    _contentListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _contentListView->setBounceEnabled(true);
    _contentListView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    _contentListView->setItemsMargin(150.0f);
    _contentListView->setContentSize(Size(this->getContentSize().width, this->getContentSize().height - 300));
    _contentListView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _contentListView->setPosition(Director::getInstance()->getVisibleOrigin());
    
    this->addChild(_contentListView);
    auto profileLayout = MeHQProfileDetails::create();
    profileLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _contentListView->pushBackCustomItem(profileLayout);
    
    auto galleryLayout = MeHQGallery::create();
    galleryLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _contentListView->pushBackCustomItem(galleryLayout);
    
    auto favouriteLayout = MeHQFavourites::create();
    favouriteLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    favouriteLayout->setRefreshCallback([this](){
        this->refreshFavouritesLayout();
    });
    _contentListView->pushBackCustomItem(favouriteLayout);
    
    auto messageList = MeHQMessages::create();
    messageList->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    messageList->setRefreshCallback([this](){
        this->refreshMessagesLayout();
    });
    _contentListView->addChild(messageList);
    
    auto downloadsLayout = MeHQDownloads::create();
    downloadsLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _contentListView->pushBackCustomItem(downloadsLayout);
    
    return true;
}

void MeHQ::onEnter()
{
    Super::onEnter();
}

void MeHQ::refreshFavouritesLayout()
{
    float scrollPercent = _contentListView->getScrolledPercentVertical();
    _contentListView->removeItem(2);
    auto favouriteLayout = MeHQFavourites::create();
    favouriteLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    favouriteLayout->setRefreshCallback([this](){
        this->refreshFavouritesLayout();
    });
    _contentListView->insertCustomItem(favouriteLayout, 2);
    
    _contentListView->forceDoLayout();
    _contentListView->scrollToPercentVertical(scrollPercent, 0, true);
}

void MeHQ::refreshMessagesLayout()
{
    //float scrollPercent = _contentListView->getScrolledPercentVertical();
    _contentListView->forceDoLayout();
    //_contentListView->scrollToPercentVertical(scrollPercent, 0, true);
}

NS_AZOOMEE_END
