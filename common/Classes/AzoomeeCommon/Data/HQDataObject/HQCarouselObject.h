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
    std::map<std::string, std::string> _images;
    
public:
    HQCarouselObject();
    static HQCarouselObjectRef create();
    
    void setTitle(const std::string &inputTitle);
    void addContentItemToCarousel(const HQContentItemObjectRef &contentItem); //also: contentItemData should contain size
    void addContentItemHighlight(const cocos2d::Vec2 &contentItemHighlight);
    void addImage(const std::string &key, const std::string &value);
    void setImages(const std::map<std::string, std::string> &images);
    void removeAllItemsFromCarousel();
    
    std::string getTitle() const;
    std::vector<HQContentItemObjectRef> getContentItems();
    std::vector<cocos2d::Vec2> getContentItemHighlights();
    std::map<std::string, std::string> getImages() const;
};

NS_AZOOMEE_END

#endif
