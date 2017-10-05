#ifndef Azoomee_HQDataObject_h
#define Azoomee_HQDataObject_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "HQCarouselObject.h"
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQDataObject
{
private:
    std::string _hqType = "";
    std::string _hqUrl = "";
    bool _hqEntitlement = false;
    std::vector<HQCarouselObject> _carousels;
    std::map<std::string, HQContentItemObject> _rawContentItems;
    bool _isDownloaded = false;
    
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
    std::vector<HQCarouselObject> *getHqCarousels();
    HQContentItemObject* getContentItemForId(const std::string &contentItemId);
    
    void clearContentItemRawStorage();
};

NS_AZOOMEE_END

#endif
