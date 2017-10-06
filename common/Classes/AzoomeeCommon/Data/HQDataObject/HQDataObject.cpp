#include "HQDataObject.h"

NS_AZOOMEE_BEGIN

HQDataObjectRef HQDataObject::create()
{
    return HQDataObjectRef();
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

void HQDataObject::setHqEntitlement(bool inputHqEntitlement)
{
    _hqEntitlement = inputHqEntitlement;
}

void HQDataObject::addCarusoelToHq(HQCarouselObjectRef inputCarouselData)
{
    _carousels.push_back(inputCarouselData);
}

std::string HQDataObject::getHqType() const
{
    return _hqType;
}

std::string HQDataObject::getHqUrl() const
{
    return _hqUrl;
}

bool HQDataObject::getHqEntitlement() const
{
    return _hqEntitlement;
}

std::vector<HQCarouselObjectRef> HQDataObject::getHqCarousels()
{
    return _carousels;
}

void HQDataObject::addContentItemToRawStorage(const std::string &contentItemId, const HQContentItemObjectRef &inputContentItemObject)
{
    _rawContentItems[contentItemId] = inputContentItemObject;
}

HQContentItemObjectRef HQDataObject::getContentItemForId(const std::string &contentItemId) const
{
    if(contentItemId == "")
    {
        return nullptr;
    }
    
    return _rawContentItems.at(contentItemId);
}

void HQDataObject::clearContentItemRawStorage()
{
    _rawContentItems.clear();
}

NS_AZOOMEE_END
