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

NS_AZOOMEE_BEGIN

class DropdownContentHolder : public ContentTileHolder, public ImageDownloaderDelegate
{
    typedef ContentTileHolder Super;
    typedef std::function<void()> OnResizeCallback;
private:
    
    static const cocos2d::Rect kBgCapInsets;
    
    OnResizeCallback _resizeCallback = nullptr;
    
    bool _open = false;
    bool _resizing = false;
    
    float _closedHeight;
    float _openHeight;
    cocos2d::Size _contentGridSize;
    
    cocos2d::ui::Scale9Sprite* _clippingStencil = nullptr;
    cocos2d::ClippingNode* _bgClipper = nullptr;
    
    cocos2d::ui::ImageView* _bgPattern = nullptr;
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
    
    
    void createTitleLayout();
    void createContentLayout();
    void updateContent();
    void resizeContent();
    
public:
    static const float kTileSpacing;
    
    void setPatternColour(const cocos2d::Color3B& colour);
    void setFrameColour(const cocos2d::Color3B& colour);
    
    void toggleOpened(bool open);
    void setOnResizeCallback(const OnResizeCallback& callback);
    
    bool isOpen() const;
    bool isResizing() const;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setContentItemData(const HQCarouselObjectRef& contentData) override;
    
    CREATE_FUNC(DropdownContentHolder);
    
    //delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* DropdownContentHolder_h */
