#ifndef Azoomee_HQDataObject_h
#define Azoomee_HQDataObject_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "HQCarouselObject.h"
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQDataObject;
typedef std::shared_ptr<HQDataObject> HQDataObjectRef;

class HQDataObject
{
private:
    std::string _hqType = "";
    std::string _hqUrl = "";
    bool _hqEntitlement = false;
    std::vector<HQCarouselObjectRef> _carousels;
    std::map<std::string, HQContentItemObjectRef> _rawContentItems;
    bool _isDownloaded = false;
    std::map<std::string, std::string> _images;
    
public:
    HQDataObject();
    static HQDataObjectRef create();
    
    void setHqType(const std::string &inputHqType);
    void setHqUrl(const std::string &inputHqUrl);
    void setHqEntitlement(bool inputHqEntitlement);
    void addCarusoelToHq(const HQCarouselObjectRef &inputCarouselData);
    void addCarusoelToHqFront(const HQCarouselObjectRef &inputCarouselData);
    void addContentItemToRawStorage(const std::string &contentItemId, const HQContentItemObjectRef &inputContentItemObject);
    void setImages(const std::map<std::string, std::string> &images);
    
    std::string getHqType() const;
    std::string getHqUrl() const;
    bool getHqEntitlement() const;
    std::vector<HQCarouselObjectRef> getHqCarousels();
    HQContentItemObjectRef getContentItemForId(const std::string &contentItemId) const;
    std::map<std::string, std::string> getImages() const;
    std::string getGroupLogo() const;
    
    void clearData();
};

NS_AZOOMEE_END

#endif
