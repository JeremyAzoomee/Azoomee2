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
#include "NavigationLayer.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemPool.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "MeHQ.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    ui::Text* heading = ui::Text::create(StringMgr::getInstance()->getStringForKey(MEHQ_HEADING_FAVOURITES), Style::Font::Regular, 75);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setContentSize(Size(visibleSize.width, spaceAboveCarousel));
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    this->addChild(heading);
    
    Sprite* icon = Sprite::create("res/meHQ/title_icon_my_favourites.png");
    icon->setAnchorPoint(Vec2(1.5f,0.25f));
    icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    heading->addChild(icon);
    
    const auto& favList = FavouritesManager::getInstance()->getFavouriteContent();
    
    if(favList.size() > 0)
    {
        Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
        float unitWidth = (visibleSize.width - 2 * sideMargin) / unitsOnScreen;
        float unitMultiplier = unitWidth / contentItemSize.width;

    
        _carouselLayout = ui::Layout::create();
        _carouselLayout->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
        _carouselLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
        float lowestElementYPosition = 0;
    
        for(int elementIndex = 0; elementIndex < favList.size(); elementIndex++)
        {
            auto hqSceneElement = HQSceneElement::create();
            hqSceneElement->setCategory(ConfigStorage::kMeHQName);
            hqSceneElement->setItemData(favList[elementIndex]);
            hqSceneElement->setElementRow(-2);
            hqSceneElement->setElementIndex(elementIndex);
            hqSceneElement->setMargin(contentItemMargin);
            hqSceneElement->setManualSizeMultiplier(unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
            hqSceneElement->deleteButtonVisible(false);
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
            hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
            hqScene2ElementPositioner.setHighlightData(elementShape);
            hqScene2ElementPositioner.setBaseUnitSize(hqSceneElement->getContentSize());
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            hqSceneElement->setPosition(elementPosition);
            _carouselLayout->addChild(hqSceneElement);
            
            if(elementPosition.y < lowestElementYPosition)
            {
                lowestElementYPosition = elementPosition.y;
            }
        }
    
        int numPlaceholders = (unitsOnScreen * ceil((double)(favList.size()) / (double)unitsOnScreen)) - favList.size();
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
            placeholder->setContentSize(placeholder->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width));
            placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(placeholder);
            hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            placeholder->setPosition(elementPosition + Vec2(contentItemMargin/2, contentItemMargin/2));
            _carouselLayout->addChild(placeholder);
        }
    
        for(auto item : _carouselLayout->getChildren())
        {
            item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
        }
    
        _carouselLayout->setContentSize(Size(_carouselLayout->getContentSize().width, -lowestElementYPosition));
    
        this->addChild(_carouselLayout);
    
        ui::Button* editButton = ui::Button::create("res/meHQ/edit_button_favourites.png");
        editButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        editButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
        editButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                _editEnabled = !_editEnabled;
                ui::Button* button = dynamic_cast<ui::Button*>(pSender);
                if(button)
                {
                    button->loadTextureNormal(_editEnabled ? "res/meHQ/done_button_favourites.png" : "res/meHQ/edit_button_favourites.png");
                }
                for(auto item : _carouselLayout->getChildren())
                {
                    HQSceneElement* element = dynamic_cast<HQSceneElement*>(item);
                    if(element)
                    {
                        element->deleteButtonVisible(_editEnabled);
                    }
                }
            }
        });
    
        this->addChild(editButton);
    
        this->setContentSize(Size(visibleSize.width, -lowestElementYPosition + spaceAboveCarousel + editButton->getContentSize().height));
    }
    else
    {
        buildEmptyCarousel();
    }
}

void MeHQFavourites::buildEmptyCarousel()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreen();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (visibleSize.width - 2 * sideMargin) / unitsOnScreen;
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::ui::Layout* carouselLayer = ui::Layout::create();
    carouselLayer->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
    carouselLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    float lowestElementYPosition = 0;
    
    for(int elementIndex = 0; elementIndex < unitsOnScreen - 1; elementIndex++)
    {
        Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
        placeholder->setContentSize(placeholder->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width));
        placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(placeholder);
        hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
        hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
        hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        float offset = contentItemMargin/2;
        placeholder->setPosition(elementPosition + Vec2(offset, offset));
        carouselLayer->addChild(placeholder);
        
        if(elementPosition.y < lowestElementYPosition)
        {
            lowestElementYPosition = elementPosition.y;
        }
    }
    
    ui::Button* playGamesButton = ui::Button::create("res/meHQ/my_favourites_button.png");
    playGamesButton->setContentSize(playGamesButton->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / playGamesButton->getContentSize().width));
    playGamesButton->ignoreContentAdaptWithSize(false);
    playGamesButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    playGamesButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            auto baseLayer = Director::getInstance()->getRunningScene();
            if(baseLayer)
            {
                NavigationLayer *navigationLayer = dynamic_cast<NavigationLayer*>(baseLayer->getChildByName("NavigationLayer"));
                
                if(navigationLayer)
                {
                    navigationLayer->changeToScene(ConfigStorage::kVideoHQName, 0.5);
                }
            }
        }
    });
    
    HQScene2ElementPositioner hqScene2ElementPositioner;
    hqScene2ElementPositioner.setElement(playGamesButton);
    hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
    hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
    hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
    
    const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
    float offset = contentItemMargin/2;
    playGamesButton->setPosition(elementPosition + Vec2(offset, offset));
    carouselLayer->addChild(playGamesButton);
    
    for(auto item : carouselLayer->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    carouselLayer->setContentSize(Size(carouselLayer->getContentSize().width, -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
    this->setContentSize(Size(visibleSize.width, -lowestElementYPosition + spaceAboveCarousel + 50));
    
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

