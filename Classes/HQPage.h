//
//  HQPage.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/08/2019.
//

#ifndef HQPage_h
#define HQPage_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObject.h>

NS_AZOOMEE_BEGIN

class HQPage : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(HQContentItemObjectRef)> ContentSelectedCallback;
    
    bool _isPortrait = true;
    
    cocos2d::ui::Layout* _structureUIHolder = nullptr;
    cocos2d::ui::ListView* _contentListView = nullptr;
    cocos2d::ui::Layout* _staticContentLayout = nullptr;
    
    ContentSelectedCallback _contentSceletedCallback = nullptr;
    
public:
  
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onSizeChanged() override;
    
    void setContentSelectedCallback(const ContentSelectedCallback& callback);
    
    CREATE_FUNC(HQPage);
};

NS_AZOOMEE_END

#endif /* HQPage_h */
