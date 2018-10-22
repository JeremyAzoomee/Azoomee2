//
//  VodacomMessageBoxNotification.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/10/2018.
//

#include "VodacomMessageBoxNotification.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void VodacomMessageBoxNotification::onEnter()
{
	Super::onEnter();
	
	_messageBox->setContentSize(Size(_visibleSize.width - 100, 855));
	
	_heading = Label::createWithTTF(_headingText, Style::Font::Regular, 91);
	_heading->setNormalizedPosition(Vec2(0.5f, 0.75f));
	_heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_heading->setHorizontalAlignment(TextHAlignment::CENTER);
	_heading->setOverflow(Label::Overflow::SHRINK);
	_heading->setDimensions(_messageBox->getContentSize().width * 0.6f, 200);
	_messageBox->addChild(_heading);
	
	Sprite* tick = Sprite::create("res/vodacom/tick.png");
	tick->setAnchorPoint(Vec2(1.5f,1.0f));
	tick->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_heading->addChild(tick);
	
	_oomee = Sprite::create("res/vodacom/notification_oomee.png");
	_oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_messageBox->addChild(_oomee);
}

void VodacomMessageBoxNotification::setHeading(const std::string &heading)
{
	_headingText = heading;
	if(_heading)
	{
		_heading->setString(_headingText);
	}
}

NS_AZOOMEE_END
