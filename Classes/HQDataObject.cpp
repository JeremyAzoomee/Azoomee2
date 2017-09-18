#include "HQDataObject.h"

NS_AZOOMEE_BEGIN

HQDataObject HQDataObject::create()
{
    return this;
}

HQDataObject::HQDataObject()
{
}

void HQDataObject::setHqType(const std::string &inputHqType)
{
    hqType = inputHqType;
}

void HQDataObject::setHqUrl(const std::string &inputHqUrl)
{
    hqUrl = inputHqUrl;
}

void HQDataObject::setHqEntitlement(const bool inputHqEntitlement)
{
    hqEntitlement = inputHqEntitlement;
}

void HQDataObject::addCarusoelToHq(HQCarouselObject inputCarouselData)
{
    carousels.push_back(inputCarouselData);
}

std::string HQDataObject::getHqType()
{
    return hqType;
}

std::string HQDataObject::getHqUrl()
{
    return hqUrl;
}

bool HQDataObject::getHqEntitlement()
{
    return hqEntitlemen;
}

std::vector<HQCarouselObject> HQDataObject::getHqCarousels()
{
    return hqCarousels;
}

NS_AZOOMEE_END
