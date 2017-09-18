#ifndef Azoomee_HQDataObject_h
#define Azoomee_HQDataObject_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class HQData
{
private:
    
    std::string hqType;
    std::string hqUrl;
    bool hqEntitlement = false;
    std::vector<std::string> carusoels; //to be replaced with carusoelDataObject
    
    // no direct construction
    HQDataObject();
    
public:
    
    static HQData create();
    
    void setHqType(const std::string &inputHqType);
    void setHqUrl(const std::string &inputHqUrl);
    void setHqEntitlement(const bool inputHqEntitlement);
    void addCarusoelToHq(const std::string &inputCarusoelData); //to be replaced with carusoelDataObject
    
    std::string getHqType();
    std::string getHqUrl();
    bool getHqEntitlement();
    std::vector<std::string> getHqCarusoels(); //to be replaced with carusoelDataObject
    
};

NS_AZOOMEE_END

#endif
