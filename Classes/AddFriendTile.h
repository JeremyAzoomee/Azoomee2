//
//  AddFriendTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 24/10/2019.
//

#ifndef AddFriendTile_h
#define AddFriendTile_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/DynamicText.h>
#include <TinizineCommon/ImageDownloader/ImageDownloader.h>
#include "AzoomeeChat/Data/Friend.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class AddFriendTile : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void()> SelectedCallback;
private:
    static const float kHeightScale;
    static const float kFrameThickness;
    
    cocos2d::ui::ImageView* _plusIcon = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::Sprite* _clippingStencil = nullptr;
    cocos2d::ui::ImageView* _pattern = nullptr;
    cocos2d::ui::ImageView* _bgColour = nullptr;
    cocos2d::ui::ImageView* _frame = nullptr;
    cocos2d::LayerRadialGradient* _circleGradient = nullptr;
    
    DynamicText* _text = nullptr;
    
    SelectedCallback _selectedCallback = nullptr;
    
    void resizeContent();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setTileWidth(float width);
    
    void setSelectedCallback(const SelectedCallback& callback);
    
    CREATE_FUNC(AddFriendTile);
    
};

NS_AZ_END

#endif /* AddFriendTile_h */
