//
//  TutorialGuide.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 30/04/2019.
//

#include "TutorialGuide.h"
#include "../Data/Child/ChildDataProvider.h"
#include "../ImageDownloader/RemoteImageSprite.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool TutorialGuide::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_frame = Sprite::create("res/tutorial/guide_frame.png");
	
	const Size& contentSize = _frame->getContentSize();
	this->setContentSize(contentSize);
	_frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_frame);
	
	_clippingNode = ClippingNode::create();
	_clippingNode->setContentSize(contentSize);
	_clippingNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_clippingNode->setPosition(contentSize/2);
	this->addChild(_clippingNode);
	
	_stencil = DrawNode::create();
	_stencil->setContentSize(contentSize);
	// Circle to cover the main area
	const float circleResolution = 64.0f;
	_stencil->drawSolidCircle(Vec2(contentSize.width * 0.5f, contentSize.height * 0.5f),
								  150.0f, 0.0f, circleResolution,
								  Color4F::GREEN);
	// A rect above the circle half way point, since some oomees have ears sticking outside the circle
	_stencil->drawSolidRect(Vec2(contentSize.width * 0.0f, contentSize.height * 0.6f),
								Vec2(contentSize.width * 1.0f, contentSize.height * 1.5f),
								Color4F::GREEN);
	_clippingNode->setStencil(_stencil);
	
	if(ChildDataProvider::getInstance()->isChildLoggedIn())
	{
		RemoteImageSprite* avatar = RemoteImageSprite::create();
		avatar->setKeepAspectRatio(true);
		avatar->initWithUrlAndSizeWithoutPlaceholder(ChildDataProvider::getInstance()->getLoggedInChild()->getAvatar(), contentSize);
		avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		avatar->setNormalizedPosition(Vec2(0.5,0.65));
		_avatar = avatar;
		_clippingNode->addChild(_avatar);
	}
	else
	{
		_avatar = Sprite::create("res/defaultOomees/oomee_11.png");
		_avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_avatar->setNormalizedPosition(Vec2(0.5,0.65));
		_avatar->setContentSize(contentSize * 1.1f);
		_clippingNode->addChild(_avatar);
	}
	
	return true;
}

void TutorialGuide::onEnter()
{
	Super::onEnter();
}

NS_AZOOMEE_END
