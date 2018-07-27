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
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    int isPortrait = visibleSize.width < visibleSize.height;
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    ui::Text* heading = ui::Text::create("My Favourites", Style::Font::Regular, 100);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setContentSize(Size(visibleSize.width, kSpaceAboveCarousel[isPortrait]));
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
    this->addChild(heading);
    
    const auto& favList = FavouritesManager::getInstance()->getFavouriteContent();
    
    if(favList.size() > 0)
    {
        Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
        float unitWidth = (visibleSize.width - 2 * kSideMarginSize[isPortrait]) / kUnitsOnScreen[isPortrait];
        float unitMultiplier = unitWidth / contentItemSize.width;

    
        _carouselLayout = ui::Layout::create();
        _carouselLayout->setContentSize(Size(visibleSize.width - 2 * kSideMarginSize[isPortrait], 0));
        _carouselLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
        float lowestElementYPosition = 0;
    
        for(int elementIndex = 0; elementIndex < favList.size(); elementIndex++)
        {
            auto hqSceneElement = HQSceneElement::create();
            hqSceneElement->setCategory(ConfigStorage::kMeHQName);
            hqSceneElement->setItemData(favList[elementIndex]);
            hqSceneElement->setElementRow(-2);
            hqSceneElement->setElementIndex(elementIndex);
            hqSceneElement->setMargin(kContentItemMargin[isPortrait]);
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
    
        int numPlaceholders = (kUnitsOnScreen[isPortrait] * ceil((double)(favList.size()) / (double)kUnitsOnScreen[isPortrait])) - favList.size();
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
            placeholder->setScale(((contentItemSize.width - kContentItemMargin[isPortrait]) * unitMultiplier) / placeholder->getContentSize().width);
            placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            HQScene2ElementPositioner hqScene2ElementPositioner;
            hqScene2ElementPositioner.setElement(placeholder);
            hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
            hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
            hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
            
            const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
            
            placeholder->setPosition(elementPosition + Vec2(kContentItemMargin[isPortrait]/2, kContentItemMargin[isPortrait]/2));
            _carouselLayout->addChild(placeholder);
        }
    
        for(auto item : _carouselLayout->getChildren())
        {
            item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
        }
    
        _carouselLayout->setContentSize(Size(_carouselLayout->getContentSize().width, -lowestElementYPosition));
    
        this->addChild(_carouselLayout);
    
        ui::Button* editButton = ui::Button::create("res/buttons/MainButton.png");
        editButton->setColor(Style::Color::darkTeal);
        editButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        editButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
        editButton->setContentSize(Size(1100,editButton->getContentSize().height));
        editButton->ignoreContentAdaptWithSize(false);
        editButton->setScale9Enabled(true);
        editButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                _editEnabled = !_editEnabled;
                ui::Button* button = dynamic_cast<ui::Button*>(pSender);
                if(button)
                {
                    Label* label = dynamic_cast<Label*>(button->getChildByName("label"));
                    if(label)
                    {
                        label->setString(_editEnabled ? "Finish Editing" : "Edit My Favourites");
                    }
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
    
        Label* editButtonLabel = Label::createWithTTF("Edit My Favourites", Style::Font::Regular, editButton->getContentSize().height * 0.4f);
        editButtonLabel->setTextColor(Color4B::WHITE);
        editButtonLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        editButtonLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        editButtonLabel->setName("label");
        editButton->addChild(editButtonLabel);
    
        this->addChild(editButton);
    
        this->setContentSize(Size(visibleSize.width, -lowestElementYPosition + kSpaceAboveCarousel[isPortrait] + editButton->getContentSize().height));
    }
    else
    {
        buildEmptyCarousel();
    }
}

void MeHQFavourites::buildEmptyCarousel()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    int isPortrait = visibleSize.width < visibleSize.height;
    
    Size contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    float unitWidth = (visibleSize.width - 2 * kSideMarginSize[isPortrait]) / kUnitsOnScreen[isPortrait];
    float unitMultiplier = unitWidth / contentItemSize.width;
    
    cocos2d::ui::Layout* carouselLayer = ui::Layout::create();
    carouselLayer->setContentSize(Size(visibleSize.width - 2 * kSideMarginSize[isPortrait], 0));
    carouselLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    float lowestElementYPosition = 0;
    
    for(int elementIndex = 0; elementIndex < kUnitsOnScreen[isPortrait] - 1; elementIndex++)
    {
        Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
        placeholder->setScale(((contentItemSize.width - kContentItemMargin[isPortrait]) * unitMultiplier) / placeholder->getContentSize().width);
        placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(placeholder);
        hqScene2ElementPositioner.setCarouselLayer(carouselLayer);
        hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
        hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        float offset = kContentItemMargin[isPortrait]/2;
        placeholder->setPosition(elementPosition + Vec2(offset, offset));
        carouselLayer->addChild(placeholder);
        
        if(elementPosition.y < lowestElementYPosition)
        {
            lowestElementYPosition = elementPosition.y;
        }
    }
    
    ui::Button* playGamesButton = ui::Button::create("res/meHQ/play_games_button.png");
    playGamesButton->setScale(((contentItemSize.width - kContentItemMargin[isPortrait]) * unitMultiplier) / playGamesButton->getContentSize().width);
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
                    navigationLayer->changeToScene(ConfigStorage::kGameHQName, 0.5);
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
    float offset = kContentItemMargin[isPortrait]/2;
    playGamesButton->setPosition(elementPosition + Vec2(offset, offset));
    carouselLayer->addChild(playGamesButton);
    
    for(auto item : carouselLayer->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    carouselLayer->setContentSize(Size(carouselLayer->getContentSize().width, -lowestElementYPosition));
    
    this->addChild(carouselLayer);
    
     ui::Text* heading = ui::Text::create(StringUtils::format("Favourite content after you watch%sor play and it will appear here.", isPortrait ? "\n" : " "), Style::Font::Regular, 80);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
    heading->setContentSize(Size(visibleSize.width, 200));
    this->addChild(heading);
    
    this->setContentSize(Size(visibleSize.width, heading->getContentSize().height -lowestElementYPosition + kSpaceAboveCarousel[isPortrait] + 50));
    
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

