#ifndef TinizineCommon_RemoteImageSprite_h
#define TinizineCommon_RemoteImageSprite_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <cocos/network/HttpClient.h>
#include "../Tinizine.h"
#include "ImageDownloader.h"
#include "ImageDownloaderOnScreenChecker.h"


NS_TZ_BEGIN

class RemoteImageSprite : public cocos2d::ui::ImageView, public ImageDownloaderDelegate, OnScreenChecker
{
    typedef cocos2d::ui::ImageView Super;
public:
    CREATE_FUNC(RemoteImageSprite);
    virtual ~RemoteImageSprite();
    
    virtual bool initWithUrlAndSize(const std::string& url, const cocos2d::Size& size);
    
    ImageDownloaderRef _imageDownloaderLogic = nullptr;
    
    void startLoadingImage();
    void removeLoadedImage();
    
    void resizeImage();
    
    void setKeepAspectRatio(bool keepAspectRatio);
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
private:
    RemoteImageSprite();
    
    // - RemoteImageSpriteDelegate
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
    
    void addPlaceHolderImage(const std::string& type, const cocos2d::Size& contentSize, const cocos2d::Vec2& shape);
    void onEnter() override;
    void onExit() override;
    void imageAddedToCache(const std::string& localPath);
    
    std::string _imageUrl;
    cocos2d::ClippingNode* _clippingNode = nullptr;
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    cocos2d::Sprite* _loadedImage = nullptr;
    
    bool _keepAspectRatio = false;
};
  
NS_TZ_END

#endif
