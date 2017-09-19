#include "HQCarouselObject.h"

NS_AZOOMEE_BEGIN

HQCarouselObject HQCarouselObject::create()
{
    return HQCarouselObject();
}

HQCarouselObject::HQCarouselObject()
{
}

void HQCarouselObject::setTitle(const std::string &inputTitle)
{
    title = inputTitle;
}

void HQCarouselObject::addContentItemToCarousel(HQContentItemObject* contentItem)
{
    contentItems.push_back(contentItem);
}

void HQCarouselObject::addContentItemHighlight(cocos2d::Vec2 contentItemHighlight)
{
    contentItemHighlights.push_back(contentItemHighlight);
}

std::string HQCarouselObject::getTitle()
{
    return title;
}

std::vector<HQContentItemObject *> HQCarouselObject::getContentItems()
{
    return contentItems;
}

std::vector<cocos2d::Vec2> HQCarouselObject::getContentItemHighlights()
{
    return contentItemHighlights;
}

NS_AZOOMEE_END
