//
//  GalleryHQScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 30/01/2019.
//

#include "GalleryHQScene.h"
#include "MeHQGallery.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool GalleryHQScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_type = HQSceneType::ART_HQ;
	
	return true;
}

void GalleryHQScene::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
	_contentListView = ui::ListView::create();
	_contentListView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_contentListView->setBounceEnabled(true);
	_contentListView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
	_contentListView->setItemsMargin(150.0f);
	_contentListView->setTopPadding(100.0f);
	_contentListView->setBottomPadding(100.0f);
	_contentListView->setContentSize(Size(contentSize.width, contentSize.height - ((ConfigStorage::getInstance()->isDeviceIphoneX() && contentSize.width < contentSize.height) ? 400 : 300)));
	_contentListView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_contentListView->setSwallowTouches(true);
	this->addChild(_contentListView);
	
	Sprite* verticalScrollGradient = Sprite::create("res/decoration/TopNavGrad.png");
	verticalScrollGradient->setAnchorPoint(Vec2(0.5, 0.9));
	verticalScrollGradient->setScaleX(contentSize.width / verticalScrollGradient->getContentSize().width);
	verticalScrollGradient->setColor(Color3B::BLACK);
	verticalScrollGradient->setPosition(Vec2(contentSize.width / 2, _contentListView->getContentSize().height));
	this->addChild(verticalScrollGradient);
	
	auto galleryLayout = MeHQGallery::create();
	galleryLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
	_contentListView->pushBackCustomItem(galleryLayout);
	
	Super::onEnter();
}

void GalleryHQScene::onTutorialStateChanged(const std::string& stateId)
{
	
}

NS_AZOOMEE_END
