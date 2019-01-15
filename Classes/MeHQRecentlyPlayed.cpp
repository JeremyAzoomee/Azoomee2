//
//  MeHQRecentlyPlayed.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 09/01/2019.
//

#include "MeHQRecentlyPlayed.h"
#include "HQSceneElement.h"
#include "HQScene2ElementPositioner.h"
#include "RecentlyPlayedManager.h"
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

bool MeHQRecentlyPlayed::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	
	const float spaceAboveCarousel = HQDataProvider::getInstance()->getSpaceAboveCarousel();
	const float sideMargin = HQDataProvider::getInstance()->getSideMargin();
	const int unitsOnScreen = HQDataProvider::getInstance()->getUnitsOnScreenMeHQ();
	const float contentItemMargin = HQDataProvider::getInstance()->getContentItemMargin();
	
	this->setContentSize(Size(visibleSize.width, 0));
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	const auto& itemList = RecentlyPlayedManager::getInstance()->getRecentlyPlayedContentForHQ(ConfigStorage::kMeHQName);
	
	if(itemList.size() > 0)
	{
		ui::Text* heading = ui::Text::create(_("LAST PLAYED"), Style::Font::Regular(), 75);
		heading->setTextHorizontalAlignment(TextHAlignment::CENTER);
		heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		heading->setContentSize(Size(visibleSize.width, spaceAboveCarousel));
		heading->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
		this->addChild(heading);
		
		const Size& contentItemSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(ConfigStorage::kGameHQName);
		const float unitWidth = (visibleSize.width - 2 * sideMargin - contentItemMargin / 2.0f) / unitsOnScreen;
		const float unitMultiplier = unitWidth / contentItemSize.width;
		
		
		_carouselLayout = ui::Layout::create();
		_carouselLayout->setContentSize(Size(visibleSize.width - 2 * sideMargin, 0));
		_carouselLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
		
		float lowestElementYPosition = 0;
		
		const Vec2& elementShape = Vec2(1,1);
		
		for(int elementIndex = 0; elementIndex < itemList.size(); elementIndex++)
		{
			auto hqSceneElement = HQSceneElement::create();
			hqSceneElement->setCategory(ConfigStorage::kMeHQName);
			hqSceneElement->setItemData(itemList[elementIndex]);
			hqSceneElement->setElementRow(-1);
			hqSceneElement->setElementIndex(elementIndex);
			hqSceneElement->setMargin(contentItemMargin);
			hqSceneElement->setManualSizeMultiplier(unitMultiplier); //overriding default configuration contentItem sizes. Ideally this *should* go away when only the new hub is present everywhere.
			
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
		
		int numPlaceholders = (unitsOnScreen * ceil((double)(itemList.size()) / (double)unitsOnScreen)) - itemList.size();
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
		
		this->setContentSize(Size(visibleSize.width, -lowestElementYPosition + spaceAboveCarousel));
	}
	return true;
}

void MeHQRecentlyPlayed::onEnter()
{
	Super::onEnter();
}

void MeHQRecentlyPlayed::onExit()
{
	Super::onExit();
}

void MeHQRecentlyPlayed::onSizeChanged()
{
	Super::onSizeChanged();
}

NS_AZOOMEE_END
