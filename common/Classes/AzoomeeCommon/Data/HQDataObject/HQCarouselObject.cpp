#include "HQCarouselObject.h"

NS_AZOOMEE_BEGIN

MutableHQCarouselObjectRef MutableHQCarouselObject::create()
{
    return std::make_shared<MutableHQCarouselObject>();
}

HQCarouselObject::HQCarouselObject()
{
}

void MutableHQCarouselObject::setTitle(const std::string &inputTitle)
{
    _title = inputTitle;
}

void MutableHQCarouselObject::addContentItemToCarousel(const HQContentItemObjectRef &contentItem)
{
    _contentItems.push_back(contentItem);
}

void MutableHQCarouselObject::addContentItemsToCarousel(const std::vector<HQContentItemObjectRef>& itemList)
{
    for(auto item : itemList)
    {
        _contentItems.push_back(item);
    }
}

void MutableHQCarouselObject::addContentItemHighlight(const cocos2d::Vec2 &contentItemHighlight)
{
    _contentItemHighlights.push_back(contentItemHighlight);
}

void MutableHQCarouselObject::setIcon(const std::string &icon)
{
    _icon = icon;
}

void MutableHQCarouselObject::setColour(const cocos2d::Color4B &colour)
{
    _colour = colour;
}

void MutableHQCarouselObject::removeAllItemsFromCarousel()
{
    _contentItems.clear();
    _contentItemHighlights.clear();
    _icon = "";
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

std::string HQCarouselObject::getIcon() const
{
    return _icon;
}

cocos2d::Color4B HQCarouselObject::getColour() const
{
    return _colour;
}

NS_AZOOMEE_END
