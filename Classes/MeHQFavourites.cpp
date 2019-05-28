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
#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
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
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreenMeHQ();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    this->setContentSize(Size(visibleSize.width, 0));
    setLayoutType(ui::Layout::Type::VERTICAL);
	
	Sprite* icon = Sprite::create("res/meHQ/title_icon_my_favourites.png");
	icon->setAnchorPoint(Vec2(1.5f,0.35f));
	icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	
    ui::Text* heading = ui::Text::create(_("MY FAVOURITES"), Style::Font::Regular(), 75);
    heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
    heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    heading->setContentSize(Size(visibleSize.width, spaceAboveCarousel));
    heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(icon->getContentSize().width* 0.75f,0,0,50)));
    this->addChild(heading);
    heading->addChild(icon);
    
    const auto& favList = FavouritesManager::getInstance()->getFavouriteContent();
    
    if(favList.size() > 0)
    {
        const Size& contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
        const float unitWidth = (visibleSize.width - 2 * sideMargin - contentItemMargin / 2.0f) / unitsOnScreen;
        const float unitMultiplier = unitWidth / contentItemSize.width;

    
        _carouselLayout = ui::Layout::create();
        _carouselLayout->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
        _carouselLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
        float lowestElementYPosition = 0;
    	const Vec2& elementShape = Vec2(1,1);
        for(int elementIndex = 0; elementIndex < favList.size(); elementIndex++)
        {
            auto hqSceneElement = HQSceneElement::create();
            hqSceneElement->setCategory(ConfigStorage::kMeHQName);
            hqSceneElement->setItemData(favList[elementIndex]);
            hqSceneElement->setElementRow(-2);
            hqSceneElement->setElementIndex(elementIndex);
            hqSceneElement->setMargin(contentItemMargin);
            hqSceneElement->setManualSizeMultiplier(unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
            hqSceneElement->deleteButtonVisible(_editEnabled);
            hqSceneElement->setDeleteButtonCallback([&](const HQContentItemObjectRef& contentItem){
				_targetDeleteItem = contentItem;
				_deleteItemMessageBox = ConfirmCancelMessageBox::createWithParams(_("Delete?"), "res/buttons/confirm_bin.png", "res/buttons/confirm_x_2.png");
				_deleteItemMessageBox->setDelegate(this);
				Director::getInstance()->getRunningScene()->addChild(_deleteItemMessageBox);
            });
            
            hqSceneElement->addHQSceneElement();
            
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
		Size placeholderSize;
        for(int i = 0; i < numPlaceholders; i++)
        {
            Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
			if(i == 0)
			{
				placeholderSize = placeholder->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width);
			}
			placeholder->setContentSize(placeholderSize);
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
    
        ui::Button* editButton = ui::Button::create(_editEnabled ? "res/meHQ/done_button_favourites.png" : "res/meHQ/edit_button_favourites.png");
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
	
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
}

void MeHQFavourites::buildEmptyCarousel()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
    const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
    const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreenMeHQ();
    const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
    
    const Size& contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
    const float unitWidth = (visibleSize.width - 2 * sideMargin - contentItemMargin / 2.0f) / unitsOnScreen;
    const float unitMultiplier = unitWidth / contentItemSize.width;
    
    _carouselLayout = ui::Layout::create();
    _carouselLayout->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
    _carouselLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    float lowestElementYPosition = 0;
    Size placeholderSize;
    for(int elementIndex = 0; elementIndex < unitsOnScreen - 1; elementIndex++)
    {
        Sprite* placeholder = Sprite::create("res/contentPlaceholders/placeholder_thumbnail_1_1.png");
		if(elementIndex == 0)
		{
			placeholderSize = placeholder->getContentSize() * (((contentItemSize.width - contentItemMargin) * unitMultiplier) / placeholder->getContentSize().width);
		}
		placeholder->setContentSize(placeholderSize);
        placeholder->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        HQScene2ElementPositioner hqScene2ElementPositioner;
        hqScene2ElementPositioner.setElement(placeholder);
        hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
        hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
        hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
        
        const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
        float offset = contentItemMargin/2;
        placeholder->setPosition(elementPosition + Vec2(offset, offset));
        _carouselLayout->addChild(placeholder);
        
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
			HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kVideoHQName);
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
        }
    });
    
    HQScene2ElementPositioner hqScene2ElementPositioner;
    hqScene2ElementPositioner.setElement(playGamesButton);
    hqScene2ElementPositioner.setCarouselLayer(_carouselLayout);
    hqScene2ElementPositioner.setHighlightData(Vec2(1,1));
    hqScene2ElementPositioner.setBaseUnitSize(contentItemSize * unitMultiplier);
    
    const cocos2d::Point &elementPosition = hqScene2ElementPositioner.positionHQSceneElement();
    float offset = contentItemMargin/2;
    playGamesButton->setPosition(elementPosition + Vec2(offset, offset));
    _carouselLayout->addChild(playGamesButton);
    
    for(auto item : _carouselLayout->getChildren())
    {
        item->setPosition(item->getPosition() - Vec2(0,lowestElementYPosition));
    }
    
    _carouselLayout->setContentSize(Size(_carouselLayout->getContentSize().width, -lowestElementYPosition));
    
    this->addChild(_carouselLayout);
    
    this->setContentSize(Size(visibleSize.width, -lowestElementYPosition + spaceAboveCarousel + 50));
    
}

void MeHQFavourites::onExit()
{
	TutorialController::getInstance()->unRegisterDelegate(this);
	_targetDeleteItem = nullptr;
	if(_deleteItemMessageBox)
	{
		_deleteItemMessageBox->removeFromParent();
		_deleteItemMessageBox = nullptr;
	}
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

void MeHQFavourites::setEditEnabled(bool enabled)
{
    _editEnabled = enabled;
}

bool MeHQFavourites::getEditEnabled() const
{
    return _editEnabled;
}

void MeHQFavourites::enableButtons(bool enable)
{
	if(_carouselLayout)
	{
		for(auto item : _carouselLayout->getChildren())
		{
			HQSceneElement* content = dynamic_cast<HQSceneElement*>(item);
			if(content)
			{
				content->setTouchDisabled(!enable);
			}
			else
			{
				ui::Button* button = dynamic_cast<ui::Button*>(item);
				if(button)
				{
					button->setTouchEnabled(enable);
				}
			}
		}
	}
}

// delegate functions

void MeHQFavourites::onConfirmPressed(ConfirmCancelMessageBox *pSender)
{
	if(_targetDeleteItem)
	{
		FavouritesManager::getInstance()->removeFromFavourites(_targetDeleteItem);
		if(_refreshCallback)
		{
			_refreshCallback();
		}
	}
	_targetDeleteItem = nullptr;
}

void MeHQFavourites::onCancelPressed(ConfirmCancelMessageBox *pSender)
{
	_targetDeleteItem = nullptr;
	_deleteItemMessageBox = nullptr;
	pSender->removeFromParent();
}

void MeHQFavourites::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END

