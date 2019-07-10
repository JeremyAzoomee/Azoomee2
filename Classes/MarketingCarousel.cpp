//
//  MarketingCarousel.cpp
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#include "MarketingCarousel.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>

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
	
	_carousel = ui::ListView::create();
	_carousel->setSizeType(SizeType::PERCENT);
	_carousel->setSizePercent(Vec2(1.0f,1.0f));
	_carousel->setDirection(ui::PageView::Direction::HORIZONTAL);
	_carousel->ignoreContentAdaptWithSize(false);
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
	setPageData(_pageData);
}

void MarketingCarousel::setPageData(const std::vector<MarketingPageData> data)
{
	_pageData = data;
	_carousel->removeAllItems();
	for(const auto& page : _pageData)
	{
		addPage(page);
	}
}

void MarketingCarousel::addPage(const MarketingPageData &data)
{
	ui::Layout* page = ui::Layout::create();
	page->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//page->setSizeType(SizeType::PERCENT);
	//page->setSizePercent(Vec2(1.0f,1.0f));
	page->setContentSize(this->getContentSize());
	page->setClippingEnabled(true);
	_carousel->pushBackCustomItem(page);
	
	//RemoteImageSprite* image = RemoteImageSprite::create();
	//image->initWithUrlAndSizeWithoutPlaceholder(data.getImageUrl(), Size(2048,2048));
	ui::ImageView* image = ui::ImageView::create(data.getImageUrl());
	//image->setSizeType(SizeType::PERCENT);
	//image->setSizePercent(Vec2(1.0f,1.0f));
	image->setScale(page->getContentSize().height / image->getContentSize().height);
	image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	image->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	image->ignoreContentAdaptWithSize(false);
	page->addChild(image);
	//SetSizePercentWidthAspectRatio(image, image->get());
	
	ui::Text* titleText = ui::Text::create(_(data.getTitle()), Style::Font::Bold(), 90);
	titleText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	titleText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	titleText->setTextColor(Color4B::WHITE);
	titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
	page->addChild(titleText);
	
	ui::Text* subHeadingText = ui::Text::create(_(data.getSubHeading()), Style::Font::Regular(), 45);
	subHeadingText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,20,0,0)));
	subHeadingText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	subHeadingText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	subHeadingText->setTextColor(Color4B::WHITE);
	subHeadingText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	subHeadingText->setTextVerticalAlignment(TextVAlignment::CENTER);
	titleText->addChild(subHeadingText);
}

NS_AZOOMEE_END
