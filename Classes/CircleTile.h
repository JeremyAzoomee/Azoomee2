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

NS_AZOOMEE_BEGIN

class CircleTile : public ContentTile, public OnScreenChecker
{
    typedef ContentTile Super;
private:
    
    static const float kFrameThickness;
    
    cocos2d::ui::ImageView* _contentImage = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::Sprite* _clippingStencil = nullptr;
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(CircleTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* CircleTile_h */
