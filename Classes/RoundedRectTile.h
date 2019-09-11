//
//  RoundedRectTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 06/09/2019.
//

#ifndef RoundedRectTile_h
#define RoundedRectTile_h

#include "ContentTile.h"
#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>

NS_AZOOMEE_BEGIN

class RoundedRectTile : public ContentTile, public OnScreenChecker
{
    typedef ContentTile Super;
private:
    
    static const cocos2d::Size kDropshadowPadding;
    
    cocos2d::ui::ImageView* _dropShadow = nullptr;
    cocos2d::ui::ImageView* _contentImage = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::ui::Scale9Sprite* _clippingStencil = nullptr;
    
    cocos2d::Vec2 _imageShape = cocos2d::Vec2(1.0f, 1.0f);
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setImageShape(const cocos2d::Vec2& imageShape);
    
    CREATE_FUNC(RoundedRectTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* RoundedRectTile_h */
