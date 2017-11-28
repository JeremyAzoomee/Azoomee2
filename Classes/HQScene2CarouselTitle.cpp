#include "HQScene2CarouselTitle.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

HQScene2CarouselTitle* HQScene2CarouselTitle::createForCarousel(HQCarouselObjectRef carousel)
{
    auto hqScene2CarouselTitle = HQScene2CarouselTitle::create();
    hqScene2CarouselTitle->addChild(hqScene2CarouselTitle->createLabel(carousel->getTitle()));
    
    if(carousel->getIcon() != "")
    {
        hqScene2CarouselTitle->addChild(hqScene2CarouselTitle->createIcon(carousel->getIcon()));
    }
    
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

cocos2d::Label* HQScene2CarouselTitle::createLabel(const std::string &text)
{
    cocos2d::Label* title = createLabelRailTitle(text);
    title->setName("title");
    title->setAnchorPoint(cocos2d::Vec2(0.5, 0));
    title->setPosition(0,20);
    
    return title;
}

cocos2d::Sprite* HQScene2CarouselTitle::createIcon(const std::string &url)
{
    RemoteImageSprite* remoteImageSprite = RemoteImageSprite::create();
    remoteImageSprite->initWithUrlAndSizeWithoutPlaceholder(url, cocos2d::Size(100,100));
    remoteImageSprite->setPosition(0,0);
    
    if(this->getChildByName("title"))
    {
        this->getChildByName("title")->setPositionX(50);
        remoteImageSprite->setPositionX(-50 - this->getChildByName("title")->getBoundingBox().size.width / 2);
    }
    
    return remoteImageSprite;
}

NS_AZOOMEE_END
