#ifndef Azoomee_HQDataObject_h
#define Azoomee_HQDataObject_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include "HQCarouselObject.h"
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQDataObject
{
private:
    std::string hqType;
    std::string hqUrl;
    bool hqEntitlement = false;
    std::vector<HQCarouselObject> carousels;
    std::map<std::string, HQContentItemObject> rawContentItems;
    
public:
    HQDataObject();
    static HQDataObject create();
    
    void setHqType(const std::string &inputHqType);
    void setHqUrl(const std::string &inputHqUrl);
    void setHqEntitlement(const bool inputHqEntitlement);
    void addCarusoelToHq(HQCarouselObject inputCarouselData);
    void addContentItemToRawStorage(std::string contentItemId, HQContentItemObject inputContentItemObject);
    
    std::string getHqType();
    std::string getHqUrl();
    bool getHqEntitlement();
    std::vector<HQCarouselObject> getHqCarousels();
    HQContentItemObject* getContentItemForId(const std::string &contentItemId);
    
    void clearContentItemRawStorage();
};

NS_AZOOMEE_END

#endif
