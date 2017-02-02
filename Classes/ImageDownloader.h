#ifndef __IMAGEDOWNLOADER_SCENE_H__
#define __IMAGEDOWNLOADER_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"

class ImageDownloader : public cocos2d::Sprite
{
public:
    CREATE_FUNC(ImageDownloader);
    virtual bool initWithURLAndSize(std::string url, cocos2d::Size size);
    virtual bool initWithUrlAndSizeWithoutPlaceholder(std::string url, cocos2d::Size size);
    void addDownloadedImage(std::string fileName);
    bool aboutToExit;
    
private:
    void addPlaceHolderImage();
    void addLoadingAnimation();
    void onExitTransitionDidStart();
};

#endif
