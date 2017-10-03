#ifndef Azoomee_HQCarouselObject_h
#define Azoomee_HQCarouselObject_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQCarouselObject
{
private:
    
    std::string title;
    std::vector<HQContentItemObject *> contentItems;
    std::vector<cocos2d::Vec2> contentItemHighlights;
    
public:
    HQCarouselObject();
    static HQCarouselObject create();
    
    void setTitle(const std::string &inputTitle);
    void addContentItemToCarousel(HQContentItemObject* contentItem); //also: contentItemData should contain size
    void addContentItemHighlight(cocos2d::Vec2 contentItemHighlight);
    
    std::string getTitle();
    std::vector<HQContentItemObject *> *getContentItems();
    std::vector<cocos2d::Vec2> *getContentItemHighlights();
    
};

NS_AZOOMEE_END

#endif
