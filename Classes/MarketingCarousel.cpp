//
//  MarketingCarousel.cpp
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#include "MarketingCarousel.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void MarketingPageData::initWithData(const rapidjson::Value& data)
{
	_imageUrl = getStringFromJson("url", data);
	_title = getStringFromJson("title", data);
	_subHeading = getStringFromJson("subHeading", data);
}
std::string MarketingPageData::getImageUrl() const
{
	return _imageUrl;
}
std::string MarketingPageData::getTitle() const
{
	return _title;
}
std::string MarketingPageData::getSubHeading() const
{
	return _subHeading;
}

bool MarketingCarousel::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_carousel = ui::PageView::create();
	_carousel->setSizeType(SizeType::PERCENT);
	_carousel->setSizePercent(Vec2(1.0f,1.0f));
	_carousel->setDirection(ui::PageView::Direction::HORIZONTAL);
	_carousel->setIndicatorEnabled(true);
	this->addChild(_carousel);
	
	return true;
}

void MarketingCarousel::onEnter()
{
	Super::onEnter();
}

void MarketingCarousel::onExit()
{
	Super::onExit();
}

void MarketingCarousel::onSizeChanged()
{
	Super::onSizeChanged();
}

void MarketingCarousel::setPageData(const std::vector<MarketingPageData> data)
{
	_pageData = data;
	_carousel->removeAllPages();
	for(const auto& page : _pageData)
	{
		addPage(page);
	}
}

void MarketingCarousel::addPage(const MarketingPageData &data)
{
	ui::ImageView* image = ui::ImageView::create(data.)
}

NS_AZOOMEE_END
