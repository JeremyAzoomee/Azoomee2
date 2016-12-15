#ifndef __IMAGEDOWNLOADER_SCENE_H__
#define __IMAGEDOWNLOADER_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"

class ImageDownloader : public cocos2d::Sprite
{
public:

    virtual bool initWithURLAndSize(std::string url, cocos2d::Size size);
    
    void addPlaceHolderImage();
    void addLoadingAnimation();
    std::string getFileNameFromURL(std::string url);
    std::string getIdFromUrl(std::string url);
    std::string getImageIdPath();
    std::string getImageCachePath();
    bool findFileInLocalCache(std::string fileName);
    void downloadFileFromServer(std::string url);
    void downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    bool saveFileToServer(std::string data, std::string fileName);
    void loadFileFromLocalCacheAsync(std::string fileName);
    void removeLoadingAnimation();
    void removePlaceHolderImage();
    
    std::string fileName;
    std::string imageId;
    std::string imageCachePath;
    std::string imageIdPath;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ImageDownloader);
};

#endif // __MAINHUBBGELEMENTS__
