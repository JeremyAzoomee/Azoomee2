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
    _title = inputTitle;
}

void HQCarouselObject::addContentItemToCarousel(HQContentItemObject* contentItem)
{
    _contentItems.push_back(contentItem);
}

void HQCarouselObject::addContentItemHighlight(cocos2d::Vec2 contentItemHighlight)
{
    _contentItemHighlights.push_back(contentItemHighlight);
}

std::string HQCarouselObject::getTitle()
{
    return _title;
}

std::vector<HQContentItemObject *> *HQCarouselObject::getContentItems()
{
    return &_contentItems;
}

std::vector<cocos2d::Vec2> *HQCarouselObject::getContentItemHighlights()
{
    return &_contentItemHighlights;
}

NS_AZOOMEE_END
