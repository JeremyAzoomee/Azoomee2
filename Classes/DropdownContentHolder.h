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
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/UI/TileSprite.h>

NS_AZOOMEE_BEGIN

class DropdownContentHolder : public ContentTileHolder, public ImageDownloaderDelegate
{
    typedef ContentTileHolder Super;
    typedef std::function<void()> OnResizeCallback;
private:
    
    static const cocos2d::Rect kBgCapInsetsSmall;
    static const cocos2d::Rect kBgCapInsetsBig;
    static const float kDropdownOpenIconScale;
    static const cocos2d::Vec2 kTileAspectRatio;
    
    OnResizeCallback _resizeCallback = nullptr;
    
    bool _open = false;
    bool _resizing = false;
    
    float _closedHeight;
    float _openHeight;
    cocos2d::Size _contentGridSize;
    
    float _tileSpacing = 32.0f;
    
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::ui::Layout* _iconLayout = nullptr;
    cocos2d::ui::Layout* _iconBackground = nullptr;
    cocos2d::Sprite* _iconStencil = nullptr;
    cocos2d::ClippingNode* _iconClippingNode = nullptr;
    cocos2d::Sprite* _categoryIcon = nullptr;
    DynamicText* _categoryTitle = nullptr;
    cocos2d::ui::Layout* _titleBanner = nullptr;
    
    cocos2d::ui::Layout* _contentTileGrid = nullptr;
    cocos2d::Vector<cocos2d::ui::Layout*> _contentRows;
    cocos2d::Vector<RoundedRectTile*> _contentTiles;
    
    ImageDownloaderRef _iconDownloader = nullptr;
    
    std::string _tilePlaceholder;
    cocos2d::Rect _bgCapInset = kBgCapInsetsSmall;
    
    void createTitleLayout();
    void createContentLayout();
    void updateContent();
    void resizeContent();
    
    void setContentLayoutVisible(bool visible);
    
public:
    static const float kTileSpacingPercent;
    
    void setPatternColour(const cocos2d::Color3B& colour);
    void setFrameColour(const cocos2d::Color3B& colour);
    
    void toggleOpened(bool open);
    void setOnResizeCallback(const OnResizeCallback& callback);
    
    void setOpen(bool open);
    bool isOpen() const;
    bool isResizing() const;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setContentItemData(const HQCarouselObjectRef& contentData) override;
    
    void setTilePlaceholder(const std::string& tilePlaceholder);
    
    void setUsingBigBg(bool useBigBG);
    
    CREATE_FUNC(DropdownContentHolder);
    
    //delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* DropdownContentHolder_h */
