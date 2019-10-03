#include "HQDataObject.h"

NS_AZOOMEE_BEGIN

MutableHQDataObjectRef MutableHQDataObject::create()
{
    return std::make_shared<MutableHQDataObject>();
}

HQDataObject::HQDataObject()
{
}

void MutableHQDataObject::setHqType(const std::string &inputHqType)
{
    _hqType = inputHqType;
}

void MutableHQDataObject::setHqUrl(const std::string &inputHqUrl)
{
    _hqUrl = inputHqUrl;
}

void MutableHQDataObject::setHqEntitlement(bool inputHqEntitlement)
{
    _hqEntitlement = inputHqEntitlement;
}

void MutableHQDataObject::setImages(const std::map<std::string, std::string> &images)
{
    _images = images;
}

void MutableHQDataObject::addCarouselToHq(const HQCarouselObjectRef &inputCarouselData)
{
    _carousels.push_back(inputCarouselData);
}

void MutableHQDataObject::addCarouselToHqFront(const HQCarouselObjectRef &inputCarouselData)
{
    _carousels.insert(_carousels.begin(), inputCarouselData);
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

std::map<std::string, std::string> HQDataObject::getImages() const
{
    return _images;
}

std::string HQDataObject::getGroupLogo() const
{
    auto itt = _images.find("logo");
    return itt != _images.end() ? itt->second : "";
}

std::string HQDataObject::getGroupBanner() const
{
    auto itt = _images.find("banner");
    return itt != _images.end() ? itt->second : "";
}

void MutableHQDataObject::clearData()
{
    _rawContentItems.clear();
    _carousels.clear();
    _images.clear();
}

NS_AZOOMEE_END
