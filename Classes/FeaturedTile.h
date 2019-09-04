//
//  FeaturedTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#ifndef FeaturedTile_h
#define FeaturedTile_h

#include "ContentTile.h"
#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>

NS_AZOOMEE_BEGIN

enum class ImageScaleMode {FIT_WIDTH, FIT_HEIGHT, SHOW_ALL, FILL_ALL};

class FeaturedTile : public ContentTile, public OnScreenChecker
{
    typedef ContentTile Super;
private:
    
    cocos2d::ui::ImageView* _contentImage = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::ui::Scale9Sprite* _clippingStencil = nullptr;
    
    ImageScaleMode _scaleMode = ImageScaleMode::FIT_WIDTH;
    cocos2d::Vec2 _imageShape = cocos2d::Vec2(1.0f, 1.0f);
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setImageScaleMode(const ImageScaleMode& scaleMode);
    void setImageShape(const cocos2d::Vec2& imageShape);
    
    CREATE_FUNC(FeaturedTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* FeaturedTile_h */
