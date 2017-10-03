#include "HQDataObject.h"

NS_AZOOMEE_BEGIN

HQDataObject HQDataObject::create()
{
    return HQDataObject();
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
    return hqEntitlement;
}

std::vector<HQCarouselObject> *HQDataObject::getHqCarousels()
{
    return &carousels;
}

void HQDataObject::addContentItemToRawStorage(std::string contentItemId, HQContentItemObject inputContentItemObject)
{
    rawContentItems[contentItemId] = inputContentItemObject;
}

HQContentItemObject* HQDataObject::getContentItemForId(const std::string &contentItemId)
{
    if(contentItemId == "") HQContentItemObject::create();
    
    return &rawContentItems[contentItemId];
}

void HQDataObject::clearContentItemRawStorage()
{
    rawContentItems.clear();
}

NS_AZOOMEE_END
