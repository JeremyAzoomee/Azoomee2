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

void HQDataObject::setImages(const std::map<std::string, std::string> &images)
{
    _images = images;
}

void HQDataObject::addCarusoelToHq(const HQCarouselObjectRef &inputCarouselData)
{
    _carousels.push_back(inputCarouselData);
}

void HQDataObject::addCarusoelToHqFront(const HQCarouselObjectRef &inputCarouselData)
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
    if(_images.find("logo") != _images.end())
    {
        return _images.at("logo");
    }
    else
    {
        return "";
    }
}

void HQDataObject::clearData()
{
    _rawContentItems.clear();
    _carousels.clear();
    _images.clear();
}

NS_AZOOMEE_END
