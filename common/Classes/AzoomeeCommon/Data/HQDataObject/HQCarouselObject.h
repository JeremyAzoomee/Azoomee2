#ifndef Azoomee_HQCarouselObject_h
#define Azoomee_HQCarouselObject_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQCarouselObject;
typedef std::shared_ptr<HQCarouselObject> HQCarouselObjectRef;

class HQCarouselObject
{
private:
    
    std::string _title = "";
    std::vector<HQContentItemObjectRef> _contentItems;
    std::vector<cocos2d::Vec2> _contentItemHighlights;
    
public:
    HQCarouselObject();
    static HQCarouselObjectRef create();
    
    void setTitle(const std::string &inputTitle);
    void addContentItemToCarousel(const HQContentItemObjectRef &contentItem); //also: contentItemData should contain size
    void addContentItemHighlight(const cocos2d::Vec2 &contentItemHighlight);
    
    std::string getTitle() const;
    std::vector<HQContentItemObjectRef> getContentItems();
    std::vector<cocos2d::Vec2> getContentItemHighlights();
    
};

NS_AZOOMEE_END

#endif
