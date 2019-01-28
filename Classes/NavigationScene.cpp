//
//  NavigationScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/01/2019.
//

#include "NavigationScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool NavigationScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void NavigationScene::onEnter()
{
	
	
	
	Super::onEnter();
}

ui::Button* NavigationScene::createNavButton(const std::string& buttonText)
{
	ui::Button* button = ui::Button::create("res/decoration/whiteWindowScale9.png");
	button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	button->setContentSize(Size(300,300));
	button->setScale9Enabled(true);
	
	Label* text = Label::createWithTTF(buttonText, Style::Font::Regular(), 100);
	text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	text->setOverflow(Label::Overflow::SHRINK);
	text->setHorizontalAlignment(TextHAlignment::CENTER);
	text->setVerticalAlignment(TextVAlignment::CENTER);
	text->setDimensions(button->getContentSize().width * 0.8f, button->getContentSize().height * 0.8f);
	
	button->addChild(text);
	return button;
}

NS_AZOOMEE_END
