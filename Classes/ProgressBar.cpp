//
//  ProgressBar.cpp
//  Azoomee
//
//  Created by Macauley on 16/07/2019.
//

#include "ProgressBar.h"
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/UI/Colour.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool ProgressBar::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(Type::HORIZONTAL);
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	
	setNumberOfSteps(3);
	setProgress(1);
	
	return true;
}
void ProgressBar::onEnter()
{
	Super::onEnter();
}
void ProgressBar::onExit()
{
	Super::onExit();
}

void ProgressBar::setNumberOfSteps(int steps)
{
	if(steps < 2 || _progressIcons.size() == steps)
	{
		return;
	}
	this->removeAllChildren();
	_progressIcons.clear();
	for(int i = 0; i < steps; i++)
	{
		ui::ImageView* progressIcon = ui::ImageView::create((i == 0 || i == steps - 1) ? "res/onboarding/progress_bar_end.png" : "res/onboarding/progress_bar_middle.png");
		progressIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		progressIcon->setRotation(i == steps - 1 ? 180 : 0);
		progressIcon->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(3,0,3,0)));
		progressIcon->setColor(Colours::Color_3B::strongPink);
		progressIcon->setOpacity(100);
		this->addChild(progressIcon);
		_progressIcons.pushBack(progressIcon);
		this->setContentSize(this->getContentSize() + Size(progressIcon->getContentSize().width + 6, i == 0 ? progressIcon->getContentSize().height : 0));
	}
}
void ProgressBar::setProgress(int progress)
{
	progress = MIN(progress, (int)_progressIcons.size());
	for(int i = 0; i < _progressIcons.size(); i++)
	{
		_progressIcons.at(i)->setOpacity(i < progress ? 255 : 100);
	}
}

NS_AZ_END
