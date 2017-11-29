#include "HQScene2CarouselTitle.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

HQScene2CarouselTitle* HQScene2CarouselTitle::createWithText(const std::string &text)
{
    auto hqScene2CarouselTitle = HQScene2CarouselTitle::create();
    
    cocos2d::Label* title = createLabelRailTitle(text);
    title->setAnchorPoint(cocos2d::Vec2(0.5, 0));
    title->setPosition(0,20);
    
    hqScene2CarouselTitle->addChild(title);
    
    return hqScene2CarouselTitle;
}

bool HQScene2CarouselTitle::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}



NS_AZOOMEE_END
