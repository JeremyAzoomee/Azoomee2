//
//  MeHQFavourites.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/05/2018.
//

#include "MeHQFavourites.h"
#include "HQSceneElement.h"
#include "HQScene2ElementPositioner.h"
#include "FavouritesManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemPool.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "MeHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float MeHQFavourites::kSideMarginSize[2] = {20.0f, 10.0f};
const float MeHQFavourites::kSpaceAboveCarousel[2] = {200.0f, 200.0f};
const int MeHQFavourites::kUnitsOnScreen[2] = {4,2};
const float MeHQFavourites::kContentItemMargin[2] = {20.0f, 20.0f};

bool MeHQFavourites::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    
    return true;
}

void MeHQFavourites::onEnter()
{
    Super::onEnter();
    
    int isPortrait = Director::getInstance()->getVisibleSize().width < Director::getInstance()->getVisibleSize().height;
    
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    auto labelLayout = ui::Layout::create();
    labelLayout->setContentSize(Size(Director::getInstance()->getVisibleSize().width, kSpaceAboveCarousel[isPortrait]));
    labelLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    labelLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    this->addChild(labelLayout);
    
    ui::Text* heading = ui::Text::create("My Favourites", Style::Font::Regular, 150);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setContentSize(Size(this->getContentSize().width, kSpaceAboveCarousel[isPortrait]));
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    labelLayout->addChild(heading);
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (this->getContentSize().width - 2 * kSideMarginSize[isPortrait]) / kUnitsOnScreen[isPortrait];
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    const auto& favList = FavouritesManager::getInstance()->getFavouriteContent();
    
    cocos2d::LayerColor* carouselLayer = LayerColor::create(cocos2d::Color4B(255, 0, 0, 0), this->getContentSize().width - 2 * kSideMarginSize[isPortrait], 0);
    
    float lowestElementYPosition = 0;
    
    for(int elementIndex = 0; elementIndex < favList.size(); elementIndex++)
    {
        auto hqSceneElement = HQSceneElement::create();
        hqSceneElement->setCategory("ME HQ");
        hqSceneElement->setItemData(favList[elementIndex]);
        hqSceneElement->setElementRow(-1);
        hqSceneElement->setElementIndex(elementIndex);
        hqSceneElement->setMargin(kContentItemMargin[isPortrait]);
        hqSceneElement->setManualSizeMultiplier(unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
        hqSceneElement->deleteButtonVisible(true);
        hqSceneElement->setDeleteButtonCallback([&](const HQContentItemObjectRef& contentItem){
            FavouritesManager::getInstance()->removeFromFavourites(contentItem);
            if(_refreshCallback)
            {
                _refreshCallback();
            }
        });
        
        hqSceneElement->addHQSceneElement();
        
        Vec2 elementShape = Vec2(1,1);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(hqSceneElement);
        hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
        hqScene2ElementPositioner.setHighlightData(elementShape);
        hqScene2ElementPositioner.setBaseUnitSize(hqSceneElement->getContentSize());
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        
        hqSceneElement->setPosition(elementPosition);
        carouselLayer->addChild(hqSceneElement);
        
        if(elementPosition.y < lowestElementYPosition)
        {
            lowestElementYPosition = elementPosition.y;
        }
    }
    
    carouselLayer->setPosition(Vec2(kSideMarginSize[isPortrait], -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
    this->setContentSize(Size(this->getContentSize().width, -lowestElementYPosition + kSpaceAboveCarousel[isPortrait]));
}

void MeHQFavourites::onExit()
{
    Super::onExit();
}

void MeHQFavourites::onSizeChanged()
{
    Super::onSizeChanged();
}

void MeHQFavourites::setRefreshCallback(const RefreshLayoutCallback &callback)
{
    _refreshCallback = callback;
}

NS_AZOOMEE_END

