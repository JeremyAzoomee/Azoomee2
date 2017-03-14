#ifndef __IMAGEDOWNLOADER_SCENE_H__
#define __IMAGEDOWNLOADER_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "ImageDownloaderLogic.h"

class ImageDownloader : public cocos2d::Sprite
{
public:
    CREATE_FUNC(ImageDownloader);
    virtual bool initWithURLAndSize(std::string url, std::string type, cocos2d::Size size, cocos2d::Vec2 shape);
    virtual bool initWithUrlAndSizeWithoutPlaceholder(std::string url, cocos2d::Size size);
    void addDownloadedImage(std::string fileName);
    bool aboutToExit;
    ImageDownloaderLogic *imageDownloaderLogic;
    
private:
    void addPlaceHolderImage(std::string type, cocos2d::Size contentSize, cocos2d::Vec2 shape);
    void addLoadingAnimation();
    void onExitTransitionDidStart();
    void onExit();
    void imageAddedToCache(cocos2d::Texture2D* resulting_texture);
    bool addStarted;
    
    float identifier;
};

#endif
