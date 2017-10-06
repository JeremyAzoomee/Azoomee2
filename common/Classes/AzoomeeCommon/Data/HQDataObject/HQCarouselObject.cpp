#include "HQCarouselObject.h"

NS_AZOOMEE_BEGIN

HQCarouselObjectRef HQCarouselObject::create()
{
    return std::make_shared<HQCarouselObject>();
}

HQCarouselObject::HQCarouselObject()
{
}

void HQCarouselObject::setTitle(const std::string &inputTitle)
{
    _title = inputTitle;
}

void HQCarouselObject::addContentItemToCarousel(const HQContentItemObjectRef &contentItem)
{
    _contentItems.push_back(contentItem);
}

void HQCarouselObject::addContentItemHighlight(const cocos2d::Vec2 &contentItemHighlight)
{
    _contentItemHighlights.push_back(contentItemHighlight);
}

std::string HQCarouselObject::getTitle() const
{
    return _title;
}

std::vector<HQContentItemObjectRef> HQCarouselObject::getContentItems()
{
    return _contentItems;
}

std::vector<cocos2d::Vec2> HQCarouselObject::getContentItemHighlights()
{
    return _contentItemHighlights;
}

NS_AZOOMEE_END
