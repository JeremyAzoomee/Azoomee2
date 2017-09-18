#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

HQContentItemObject HQContentItemObject::create()
{
    return this;
}

HQContentItemObject::HQContentItemObject()
{
}

void HQContentItemObject::setTitle(const std::string &inputTitle)
{
    title = inputTitle;
}

void HQContentItemObject::setDescription(const std::string &inputDescription)
{
    description = inputDescription;
}

void HQContentItemObject::setType(const std::string &inputType)
{
    type = inputType;
}

void HQContentItemObject::setUri(const std::string &inputUri)
{
    uri = inputUri;
}

void HQContentItemObject::setEntitled(const bool inputEntitled)
{
    entitled = inputEntitled;
}

std::string HQContentItemObject::getTitle()
{
    return title;
}

std::string HQContentItemObject::getDescription()
{
    return description;
}

std::string HQContentItemObject::getType()
{
    return type;
}

std::string HQContentItemObject::getUri()
{
    return uri;
}

bool HQContentItemObject::getEntitled()
{
    return entitled;
}

bool HQContentItemObject::getNewFlag()
{
    return newFlag;
}

NS_AZOOMEE_END
