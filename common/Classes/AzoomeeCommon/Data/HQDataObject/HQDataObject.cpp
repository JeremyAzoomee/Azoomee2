#include "HQDataObject.h"

NS_AZOOMEE_BEGIN

HQDataObjectRef HQDataObject::create()
{
    return std::make_shared<HQDataObject>();
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

void HQDataObject::addImage(const std::string &key, const std::string &value)
{
    _images[key] = value;
}

void HQDataObject::setImages(const std::map<std::string, std::string> &images)
{
    _images = images;
}

void HQDataObject::addCarusoelToHq(const HQCarouselObjectRef &inputCarouselData)
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
    try
    {
        HQContentItemObjectRef item = _rawContentItems.at(contentItemId);
        return item;
    }
    catch(std::out_of_range)
    {
        return nullptr;
    }
}

std::map<std::string, std::string> HQDataObject::getImages() const
{
    return _images;
}

void HQDataObject::clearData()
{
    _rawContentItems.clear();
    _carousels.clear();
    _images.clear();
}

NS_AZOOMEE_END
