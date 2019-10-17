//
//  ArtTileHolder.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 15/10/2019.
//

#ifndef ArtTileHolder_h
#define ArtTileHolder_h

#include "ArtContentTile.h"
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class ArtTileHolder : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void()> OnResizeCallback;
    typedef std::function<void(const std::string&)> ArtContentTileCallback;
private:
    
    static const cocos2d::Vec2 kTileAspectRatio;
    static const int kClosedMaxVisibleTileCount;
    static const float kTilesPerRow;
    
    OnResizeCallback _togglePressedCallback = nullptr;
    OnResizeCallback _resizeCallback = nullptr;
    ArtContentTileCallback _editCallback = nullptr;
    ArtContentTileCallback _deleteCallback = nullptr;
    
    bool _open = false;
    bool _resizing = false;
    bool _expandable = false;
    float _contentGridPadding;
    
    float _closedHeight;
    float _openHeight;
    cocos2d::Size _contentGridSize;
    
    float _tileSpacing = 32.0f;
    
    RoundedRectSprite* _bgPattern = nullptr;
    RoundedRectSprite* _bgColour = nullptr;
    cocos2d::ui::Layout* _contentClippingLayout = nullptr;
    cocos2d::ui::Layout* _contentTileGrid = nullptr;
    cocos2d::Vector<cocos2d::ui::Layout*> _contentRows;
    cocos2d::Vector<ArtContentTile*> _contentTiles;
    cocos2d::ui::Button* _resizeToggle = nullptr;
    
    std::vector<std::string> _artFilenames;

    void createContentLayout();
    void updateContent();
    void resizeContent();
    
    cocos2d::Size calcTileSize(float rowWidth);
    
public:
    static const float kTileSpacingPercent;
    static const std::string kEmptyArtFilename;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void toggleOpened(bool open);
    void setOnResizeCallback(const OnResizeCallback& callback);
    void setToggleSelectedCallback(const OnResizeCallback& callback);
    
    bool isOpen() const;
    bool isResizing() const;
    
    void setArtFilenames(const std::vector<std::string>& artFilenames);
    
    void setEditCallback(const ArtContentTileCallback& callback);
    void setDeleteCallback(const ArtContentTileCallback& callback);
    
    CREATE_FUNC(ArtTileHolder);
    
};

NS_AZOOMEE_END

#endif /* ArtTileHolder_h */
