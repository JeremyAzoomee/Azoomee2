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
    _hqType = inputHqType;
}

void HQDataObject::setHqUrl(const std::string &inputHqUrl)
{
    _hqUrl = inputHqUrl;
}

void HQDataObject::setHqEntitlement(const bool inputHqEntitlement)
{
    _hqEntitlement = inputHqEntitlement;
}

void HQDataObject::addCarusoelToHq(HQCarouselObject inputCarouselData)
{
    _carousels.push_back(inputCarouselData);
}

std::string HQDataObject::getHqType()
{
    return _hqType;
}

std::string HQDataObject::getHqUrl()
{
    return _hqUrl;
}

bool HQDataObject::getHqEntitlement()
{
    return _hqEntitlement;
}

std::vector<HQCarouselObject> *HQDataObject::getHqCarousels()
{
    return &_carousels;
}

void HQDataObject::addContentItemToRawStorage(std::string contentItemId, HQContentItemObject inputContentItemObject)
{
    _rawContentItems[contentItemId] = inputContentItemObject;
}

HQContentItemObject* HQDataObject::getContentItemForId(const std::string &contentItemId)
{
    if(contentItemId == "")
    {
        return nullptr;
    }
    
    return &_rawContentItems[contentItemId];
}

void HQDataObject::clearContentItemRawStorage()
{
    _rawContentItems.clear();
}

NS_AZOOMEE_END
