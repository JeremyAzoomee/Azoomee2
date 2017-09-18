#ifndef Azoomee_HQCarouselObject_h
#define Azoomee_HQCarouselObject_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQCarouselObject
{
private:
    
    std::string title;
    std::vector<HQContentItemObject> contentItems;
    HQCarouselObject();
    
public:
    
    static HQCarouselObject create();
    
    void setTitle(const std::string &inputTitle);
    void addContentItemToCarousel(HQContentItemObject contentItem); //also: contentItemData should contain size
    
    std::string getTitle();
    std::vector<HQContentItemObject> getContentItems();
    
};

NS_AZOOMEE_END

#endif
