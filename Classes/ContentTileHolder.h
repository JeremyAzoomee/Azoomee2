//
//  ContentTileHolder.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#ifndef ContentTileHolder_h
#define ContentTileHolder_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/Data/HQDataObject/HQContentItemObject.h>
#include <TinizineCommon/Data/HQDataObject/HQCarouselObject.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ContentTileHolder : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(HQContentItemObjectRef, int)> ContentSelectedCallback;
    
    ContentSelectedCallback _callback = nullptr;
    HQCarouselObjectRef _contentData = nullptr;
    
    cocos2d::ui::Layout* _contentLayout = nullptr;
    
public:
    
    virtual bool init() override;
    
    void setContentSelectedCallback(const ContentSelectedCallback& callback);
    virtual void setContentItemData(const HQCarouselObjectRef& contentData);
    HQCarouselObjectRef getContentItemData() const;
    
    CREATE_FUNC(ContentTileHolder);
    
};

NS_AZ_END

#endif /* ContentTileHolder_h */
