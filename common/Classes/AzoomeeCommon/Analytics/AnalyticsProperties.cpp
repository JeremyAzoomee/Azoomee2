//
//  AnalyticsProperties.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 04/01/2018.
//

#include "AnalyticsProperties.h"

NS_AZOOMEE_BEGIN

using namespace cocos2d;

static std::auto_ptr<AnalyticsProperties> sAnalyticsPropertiesSharedInstance;

AnalyticsProperties* AnalyticsProperties::getInstance()
{
    if(!sAnalyticsPropertiesSharedInstance.get())
    {
        sAnalyticsPropertiesSharedInstance.reset(new AnalyticsProperties());
    }
    return sAnalyticsPropertiesSharedInstance.get();
}

AnalyticsProperties::~AnalyticsProperties(void)
{
    
}

bool AnalyticsProperties::init(void)
{
    return true;
}

void AnalyticsProperties::setStoredContentItemProperties(const std::map<std::string, std::string>& properties)
{
    _storedContentItemProperties = properties;
}

void AnalyticsProperties::addPropertyToStoredContentItemProperties(const std::string &key, const std::string &value)
{
    _storedContentItemProperties[key] = value;
}

std::map<std::string, std::string> AnalyticsProperties::getStoredContentItemProperties() const
{
    return _storedContentItemProperties;
}

void AnalyticsProperties::addGeneralProperty(const std::string& key, const std::string& value)
{
    _storedGeneralProperties[key] = value;
}
std::map<std::string, std::string> AnalyticsProperties::getStoredGeneralProperties() const
{
    return _storedGeneralProperties;
}

void AnalyticsProperties::setCtaWindowNodeId(const std::string& nodeId)
{
    _ctaWindowNodeId = nodeId;
}
std::string AnalyticsProperties::getCtaWindowNodeId() const
{
    return _ctaWindowNodeId;
}

void AnalyticsProperties::setCtaWindowGroupId(const std::string& groupId)
{
    _ctaWindowGroupId = groupId;
}
std::string AnalyticsProperties::getCtaWindowGroupId() const
{
    return _ctaWindowGroupId;
}

void AnalyticsProperties::setCtaSourceButton(const std::string &buttonId)
{
    _ctaSourceButton = buttonId;
}
std::string AnalyticsProperties::getCtaSourceButton() const
{
    return _ctaSourceButton;
}

void AnalyticsProperties::setCtaSourceContentId(const std::string &contentId)
{
    _ctaSourceContentId = contentId;
}
std::string AnalyticsProperties::getCtaSourceContentId() const
{
    return _ctaSourceContentId;
}

void AnalyticsProperties::setCtaSourceMediaType(const std::string &mediaType)
{
    _ctaSourceMediaType = mediaType;
}
std::string AnalyticsProperties::getCtaSourceMediaType() const
{
    return _ctaSourceMediaType;
}

void AnalyticsProperties::setOSManufacturer(const std::string& osManufacturer)
{
    _OSManufacturer = osManufacturer;
}
std::string AnalyticsProperties::getOSManufacturer() const
{
    return _OSManufacturer;
}

void AnalyticsProperties::setTimeOpenedContent(time_t timeOpenedContent)
{
    _timeOpenedContent = timeOpenedContent;
}
time_t AnalyticsProperties::getTimeOpenedContent() const
{
    return _timeOpenedContent;
}

NS_AZOOMEE_END
