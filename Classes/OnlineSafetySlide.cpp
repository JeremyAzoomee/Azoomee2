//
//  OnlineSafetySlide.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 24/08/2018.
//

#include "OnlineSafetySlide.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool OnlineSafetySlide::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/settings/rounded_rect.png");
    setBackGroundImageScale9Enabled(true);
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void OnlineSafetySlide::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
    _titleText = Label::createWithTTF(_titleString, Style::Font::Medium, 75);
    _titleText->setHorizontalAlignment(TextHAlignment::CENTER);
    _titleText->setTextColor(Color4B::BLACK);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _titleText->setWidth(contentSize.width * 0.9f);
    
    ui::Layout* titleLayout = ui::Layout::create();
    titleLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    titleLayout->setContentSize(Size(contentSize.width, _titleText->getContentSize().height));
    titleLayout->addChild(_titleText);
    this->addChild(titleLayout);
    
    _image = ui::ImageView::create(_imageFilename);
    _image->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,60,0,0)));
    float scalefactor = (contentSize.height * 0.42f) / _image->getContentSize().height;
    _image->setContentSize(_image->getContentSize() * scalefactor);
    _image->ignoreContentAdaptWithSize(false);
    this->addChild(_image);
    
    _bodyText = Label::createWithTTF(_bodyString, Style::Font::Medium, 59);
    _bodyText->setHorizontalAlignment(TextHAlignment::LEFT);
    _bodyText->setWidth(contentSize.width * 0.9f);
    _bodyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bodyText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _bodyText->setTextColor(Color4B::BLACK);
    
    ui::Layout* bodyLayout = ui::Layout::create();
    bodyLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,60,0,0)));
    bodyLayout->setContentSize(Size(contentSize.width, _bodyText->getContentSize().height));
    bodyLayout->addChild(_bodyText);
    this->addChild(bodyLayout);
    
    Super::onEnter();
}

void OnlineSafetySlide::setTitle(const std::string &text)
{
    _titleString = text;
}

void OnlineSafetySlide::setBodyText(const std::string &text)
{
    _bodyString = text;
}

void OnlineSafetySlide::setImage(const std::string &imageFilename)
{
    _imageFilename = imageFilename;
}

NS_AZOOMEE_END


