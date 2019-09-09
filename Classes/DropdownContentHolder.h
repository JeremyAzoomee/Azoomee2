//
//  DropdownContentHolder.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 09/09/2019.
//

#ifndef DropdownContentHolder_h
#define DropdownContentHolder_h

#include "ContentTileHolder.h"
#include "RoundedRectTile.h"
#include <AzoomeeCommon/UI/DynamicText.h>

NS_AZOOMEE_BEGIN

class DropdownContentHolder : public ContentTileHolder
{
    typedef ContentTileHolder Super;
private:
    
    static const cocos2d::Rect kBgCapInsets;
    
    bool _open = false;
    
    cocos2d::ui::Scale9Sprite* _clippingStencil = nullptr;
    cocos2d::ClippingNode* _bgClipper = nullptr;
    
    cocos2d::ui::ImageView* _bgPattern = nullptr;
    cocos2d::ui::ImageView* _categoryIcon = nullptr;
    DynamicText* _categoryTitle = nullptr;
    cocos2d::ui::Layout* _titleBanner = nullptr;
    
    cocos2d::ui::Layout* _contentTileGrid = nullptr;
    cocos2d::Vector<RoundedRectTile*> _contentTiles;
    
    void createTitleLayout();
    void createContentLayout();
    
public:
    static const float kTileSpacing;
    
    void setPatternColour(const cocos2d::Color3B& colour);
    void setFrameColour(const cocos2d::Color3B& colour);
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(DropdownContentHolder);
};

NS_AZOOMEE_END

#endif /* DropdownContentHolder_h */
