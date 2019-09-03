//
//  FeaturedTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#ifndef FeaturedTile_h
#define FeaturedTile_h

#include "ContentTile.h"

NS_AZOOMEE_BEGIN

enum class ImageScaleMode {FIT_WIDTH, FIT_HEIGHT};

class FeaturedTile : public ContentTile
{
    typedef ContentTile Super;
private:
    
    cocos2d::ui::ImageView* _contentImage = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::ui::Scale9Sprite* _clippingStencil = nullptr;
    
    ImageScaleMode _scaleMode = ImageScaleMode::FIT_WIDTH;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setImageScaleMode(const ImageScaleMode& scaleMode);
    
    CREATE_FUNC(FeaturedTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* FeaturedTile_h */
