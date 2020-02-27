//
//  FeaturedTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#ifndef FeaturedTile_h
#define FeaturedTile_h

#include "ContentTile.h"
#include <TinizineCommon/ImageDownloader/OnScreenChecker.h>
#include <TinizineCommon/UI/RoundedRectSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class FeaturedTile : public ContentTile, public OnScreenChecker
{
    typedef ContentTile Super;
private:
    
    RoundedRectSprite* _contentImage = nullptr;
    
    RoundedRectSprite* _lockedOverlay = nullptr;
    cocos2d::ui::ImageView* _padlock = nullptr;
    
    cocos2d::Vec2 _imageShape = cocos2d::Vec2(1.0f, 1.0f);
    
    void resizeContentImage();
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setImageShape(const cocos2d::Vec2& imageShape);
    
    void setContentItemData(const HQContentItemObjectRef& contentItem) override;
    
    void setPlaceholderFilename(const std::string& placeholder) override;
    
    CREATE_FUNC(FeaturedTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZ_END

#endif /* FeaturedTile_h */
