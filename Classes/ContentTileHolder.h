//
//  ContentTileHolder.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#ifndef ContentTileHolder_h
#define ContentTileHolder_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>

NS_AZOOMEE_BEGIN

class ContentTileHolder : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(HQContentItemObjectRef)> ContentSelectedCallback;
    
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

NS_AZOOMEE_END

#endif /* ContentTileHolder_h */
