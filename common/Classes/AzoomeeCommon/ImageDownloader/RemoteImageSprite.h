#ifndef AzoomeeCommon_RemoteImageSprite_h
#define AzoomeeCommon_RemoteImageSprite_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <cocos/network/HttpClient.h>
#include "../Azoomee.h"
#include "ImageDownloader.h"
#include "ImageDownloaderOnScreenChecker.h"


NS_AZOOMEE_BEGIN

class RemoteImageSprite : public cocos2d::Sprite, public ImageDownloaderDelegate
{
    typedef cocos2d::Sprite Super;
public:
    CREATE_FUNC(RemoteImageSprite);
    virtual ~RemoteImageSprite();
    
    virtual bool initWithURLAndSize(const std::string& url, const std::string& type, const cocos2d::Size& size, const cocos2d::Vec2& shape);
    virtual bool initWithUrlAndSizeWithoutPlaceholder(const std::string& url, const cocos2d::Size& size, bool useStencil = false);
    bool aboutToExit = false;
    ImageDownloaderRef imageDownloaderLogic;
    
    void startLoadingImage();
    void removeLoadedImage();
    
    void setAttachNewBadgeToImage();
    void resizeImage();
    
    void setKeepAspectRatio(bool keepAspectRatio);
    
private:
    RemoteImageSprite();
    
    // - RemoteImageSpriteDelegate
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
    
    void addPlaceHolderImage(std::string type, cocos2d::Size contentSize, cocos2d::Vec2 shape);
    void addLoadingAnimation();
    void removeLoadingAnimation();
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void onExit() override;
    void imageAddedToCache(cocos2d::Texture2D* resulting_texture);
    void addClippingNode(bool usingClippingNode);
    
    bool addStarted = false;
    
    float identifier = -1.0f;
    std::string imageUrl;
    cocos2d::ClippingNode* _clippingNode = nullptr;
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    cocos2d::Sprite* loadedImage = nullptr;
    
    bool _keepAspectRatio = false;
    
    ImageDownloaderOnScreenChecker *onScreenChecker = nullptr;
    
    void addNewBadgeToLoadedImage();
    bool shouldAddNewBadgeToImage = false;
};
  
NS_AZOOMEE_END

#endif
