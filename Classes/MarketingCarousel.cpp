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

const float MarketingCarousel::ktimeBetweenScrolls = 5.0f;

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
	_carousel->setTouchEnabled(false);
	_carousel->setScrollDuration(2.0f);
	this->addChild(_carousel);
	
	return true;
}

void MarketingCarousel::onEnter()
{
	scheduleUpdate();
	Super::onEnter();
}

void MarketingCarousel::onExit()
{
	unscheduleUpdate();
	Super::onExit();
}

void MarketingCarousel::onSizeChanged()
{
	Super::onSizeChanged();
	setPageData(_pageData);
}

void MarketingCarousel::update(float deltaT)
{
	if(_carousel->getItems().size() > 1)
	{
		if(_carousel->isScrolling())
		{
			return;
		}
		else
		{
			_timeTillNextScroll -= deltaT;
			if(_timeTillNextScroll <= 0)
			{
				_timeTillNextScroll = ktimeBetweenScrolls;
				auto targetIndex = (_carousel->getCurSelectedIndex() < (_carousel->getItems().size() - 1)) ? _carousel->getCurSelectedIndex() + 1 : 0;
				_carousel->scrollToItem(targetIndex , Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
				_carousel->setCurSelectedIndex((int)targetIndex);
			}
		}
	}
	
	Super::update(deltaT);
}

void MarketingCarousel::setPageData(const std::vector<MarketingPageData> data)
{
	_pageData = data;
	_carousel->removeAllItems();
	for(const auto& page : _pageData)
	{
		addPage(page);
	}
	_carousel->setCurSelectedIndex(0);
	_timeTillNextScroll = ktimeBetweenScrolls;
	forceDoLayout();
}

void MarketingCarousel::addPage(const MarketingPageData &data)
{
	ui::Layout* page = ui::Layout::create();
	page->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	page->setContentSize(this->getContentSize());
	page->setClippingEnabled(true);
	_carousel->pushBackCustomItem(page);
	
	//RemoteImageSprite* image = RemoteImageSprite::create();
	//image->initWithUrlAndSizeWithoutPlaceholder(data.getImageUrl(), Size(2048,2048));
	ui::ImageView* image = ui::ImageView::create(data.getImageUrl());
	image->setScale(page->getContentSize().height / image->getContentSize().height);
	image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	image->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	image->ignoreContentAdaptWithSize(false);
	page->addChild(image);
	
	ui::Text* titleText = ui::Text::create(_(data.getTitle()), Style::Font::Bold(), 120);
	titleText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	titleText->setNormalizedPosition(Vec2(0.5f,0.95f));
	titleText->setTextColor(Color4B::WHITE);
	titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
	Label* titleLabel = dynamic_cast<Label*>(titleText->getVirtualRenderer());
	if(titleLabel)
	{
		titleLabel->setMaxLineWidth(page->getContentSize().width * 0.8f);
	}
	page->addChild(titleText);
	
	ui::Text* subHeadingText = ui::Text::create(_(data.getSubHeading()), Style::Font::Regular(), 60);
	subHeadingText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	subHeadingText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	subHeadingText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	subHeadingText->setTextColor(Color4B::WHITE);
	subHeadingText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	subHeadingText->setTextVerticalAlignment(TextVAlignment::CENTER);
	Label* subHeadingLabel = dynamic_cast<Label*>(titleText->getVirtualRenderer());
	if(subHeadingLabel)
	{
		subHeadingLabel->setMaxLineWidth(page->getContentSize().width * 0.8f);
	}
	titleText->addChild(subHeadingText);
}

NS_AZOOMEE_END
