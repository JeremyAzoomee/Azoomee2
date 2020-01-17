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
#include "DropdownContentHolder.h"
#include "HQConstants.h"

NS_AZOOMEE_BEGIN

class HQPage : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(HQContentItemObjectRef, int, int, const std::string&)> ContentSelectedCallback;
    
    static const cocos2d::Size kCircleTileSizeLandscape;
    static const cocos2d::Size kCircleTileSizePortrait;
    static const float kFeaturedContentHeightLandscape;
    static const float kFeaturedContentHeightPortrait;
    static const float kListViewSidePadding;
    static const cocos2d::Vec2 kFocusDropDownAnchor;
    
    bool _isPortrait = true;
    
    cocos2d::ui::Layout* _structureUIHolder = nullptr;
    cocos2d::ui::ListView* _contentListView = nullptr;
    cocos2d::ui::Layout* _staticContentLayout = nullptr;
    
    ContentSelectedCallback _contentSelectedCallback = nullptr;
    
    /// The dropdown last selected for open or close, may be null
    cocos2d::ui::Layout* _focusedDropdown = nullptr;
    cocos2d::Vec2 _resizingPositionInView;
    
    void listviewDropdownResizeCallback();
    void dropdownAutoOpenCloseLogic(DropdownContentHolder* pressedDropdown, const cocos2d::Vector<DropdownContentHolder*>& dropdownHoldersInListview);
    
public:
  
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onSizeChanged() override;
    
    void setContentSelectedCallback(const ContentSelectedCallback& callback);
    
    float getScrolledPos() const;
    void setScrolledPos(float pos);
    
    CREATE_FUNC(HQPage);
};

NS_AZOOMEE_END

#endif /* HQPage_h */
