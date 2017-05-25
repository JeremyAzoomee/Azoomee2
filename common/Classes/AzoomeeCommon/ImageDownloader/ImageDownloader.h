#ifndef AzoomeeCommon_ImageDownloader_h
#define AzoomeeCommon_ImageDownloader_h

#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>
#include "../Azoomee.h"
#include "ImageDownloaderLogic.h"
#include "ImageDownloaderOnScreenChecker.h"


NS_AZOOMEE_BEGIN

class ImageDownloader : public cocos2d::Sprite, public ImageDownloaderDelegate
{
public:
    CREATE_FUNC(ImageDownloader);
    virtual ~ImageDownloader();
    
    virtual bool initWithURLAndSize(const std::string& url, const std::string& type, const cocos2d::Size& size, const cocos2d::Vec2& shape);
    virtual bool initWithUrlAndSizeWithoutPlaceholder(const std::string& url, const cocos2d::Size& size);
    bool aboutToExit;
    ImageDownloaderLogic *imageDownloaderLogic = nullptr;
    
    void startLoadingImage();
    void removeLoadedImage();
    
    void setAttachNewBadgeToImage();
    
private:
    // - ImageDownloaderDelegate
    virtual void onImageDownloadComplete(ImageDownloaderLogic* downloader) override;
    
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
