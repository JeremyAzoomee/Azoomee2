#include "HQDataObject.h"

NS_AZOOMEE_BEGIN

HQData HQDataObject::create()
{
    return this;
}

HQDataObject::HQDataObject()
{
}

void setHqType(const std::string &inputHqType)
{
    hqType = inputHqType;
}

void setHqUrl(const std::string &inputHqUrl)
{
    hqUrl = inputHqUrl;
}

void setHqEntitlement(const bool inputHqEntitlement)
{
    hqEntitlement = inputHqEntitlement;
}

void addCarusoelToHq(const std::string &inputCarusoelData); //to be replaced with carusoelDataObject

std::string getHqType();
std::string getHqUrl();
bool getHqEntitlement();
std::vector<std::string> getHqCarusoels(); //to be replaced with carusoelDataObject

NS_AZOOMEE_CHAT_END
