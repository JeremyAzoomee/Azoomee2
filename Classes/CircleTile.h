//
//  CircleTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 05/09/2019.
//

#ifndef CircleTile_h
#define CircleTile_h

#include "ContentTile.h"
#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class CircleTile : public ContentTile, public OnScreenChecker
{
    typedef ContentTile Super;
private:
    
    static const float kFrameThickness;
    
    cocos2d::ui::ImageView* _contentImage = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::Sprite* _clippingStencil = nullptr;
    cocos2d::ui::ImageView* _bgColour = nullptr;
    
    std::string _emptyImageFilename;
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setContentItemData(const HQContentItemObjectRef& contentItem) override;
    
    void setEmptyImage(const std::string& filename);
    
    CREATE_FUNC(CircleTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZ_END

#endif /* CircleTile_h */
