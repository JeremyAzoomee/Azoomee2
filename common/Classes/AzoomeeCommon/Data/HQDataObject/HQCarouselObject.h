#ifndef Azoomee_HQCarouselObject_h
#define Azoomee_HQCarouselObject_h

#include "../../Tinizine.h"
#include <cocos/cocos2d.h>
#include "HQContentItemObject.h"

NS_TZ_BEGIN

class HQCarouselObject;
typedef std::shared_ptr<HQCarouselObject> HQCarouselObjectRef;
class MutableHQCarouselObject;
typedef std::shared_ptr<MutableHQCarouselObject> MutableHQCarouselObjectRef;

class HQCarouselObject
{
protected:
    
    std::string _title = "";
    std::vector<HQContentItemObjectRef> _contentItems;
    std::vector<cocos2d::Vec2> _contentItemHighlights;
    std::string _icon = "";
    cocos2d::Color4B _colour;
    
    HQCarouselObject();
public:
    
    std::string getTitle() const;
    std::vector<HQContentItemObjectRef> getContentItems();
    std::vector<cocos2d::Vec2> getContentItemHighlights();
    std::string getIcon() const;
    cocos2d::Color4B getColour() const;
};

class MutableHQCarouselObject : public HQCarouselObject
{
public:
	static MutableHQCarouselObjectRef create();
	
	void setTitle(const std::string &inputTitle);
	void addContentItemToCarousel(const HQContentItemObjectRef &contentItem); //also: contentItemData should contain size
    void addContentItemsToCarousel(const std::vector<HQContentItemObjectRef>& itemList);
	void addContentItemHighlight(const cocos2d::Vec2 &contentItemHighlight);
	void setIcon(const std::string &icon);
    void setColour(const cocos2d::Color4B& colour);
	void removeAllItemsFromCarousel();
};

NS_TZ_END

#endif
