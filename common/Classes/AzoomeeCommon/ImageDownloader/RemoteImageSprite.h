#ifndef AzoomeeCommon_RemoteImageSprite_h
#define AzoomeeCommon_RemoteImageSprite_h

#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>
#include "../Azoomee.h"
#include "ImageDownloader.h"
#include "ImageDownloaderOnScreenChecker.h"


NS_AZOOMEE_BEGIN

class RemoteImageSprite : public cocos2d::Sprite, public ImageDownloaderDelegate
{
public:
    CREATE_FUNC(RemoteImageSprite);
    virtual ~RemoteImageSprite();
    
    virtual bool initWithURLAndSize(const std::string& url, const std::string& type, const cocos2d::Size& size, const cocos2d::Vec2& shape);
    virtual bool initWithUrlAndSizeWithoutPlaceholder(const std::string& url, const cocos2d::Size& size);
    bool aboutToExit;
    ImageDownloader *imageDownloaderLogic = nullptr;
    
    void startLoadingImage();
    void removeLoadedImage();
    
    void setAttachNewBadgeToImage();
    
private:
    // - RemoteImageSpriteDelegate
    virtual void onImageDownloadComplete(ImageDownloader* downloader) override;
    
    void addPlaceHolderImage(std::string type, cocos2d::Size contentSize, cocos2d::Vec2 shape);
    void addLoadingAnimation();
    void removeLoadingAnimation();
    void onExitTransitionDidStart() override;
    void onExit() override;
    void imageAddedToCache(cocos2d::Texture2D* resulting_texture);
    bool addStarted;
    
    float identifier;
    std::string imageUrl;
    cocos2d::Sprite* loadedImage = nullptr;
    
    void onEnter();
    ImageDownloaderOnScreenChecker *onScreenChecker = nullptr;
    
    void addNewBadgeToLoadedImage();
    bool shouldAddNewBadgeToImage;
};
  
NS_AZOOMEE_END

#endif
