#include "HQCarouselObject.h"

NS_AZOOMEE_BEGIN

HQCarouselObject HQCarouselObject::create()
{
    return this;
}

HQCarouselObject::HQCarouselObject()
{
}

void HQCarouselObject::setTitle(const std::string &inputTitle)
{
    title = inputTitle;
}

void HQCarouselObject::addContentItemToCarousel(HQContentItemObject contentItem)
{
    contentItems.push_back(contentItem);
}

std::string HQCarouselObject::getTitle()
{
    return title;
}

std::vector<HQContentItemObject> HQCarouselObject::getContentItems()
{
    return contentItems;
}

NS_AZOOMEE_END
