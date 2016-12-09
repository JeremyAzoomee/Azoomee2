#ifndef __IMAGEDOWNLOADER_SCENE_H__
#define __IMAGEDOWNLOADER_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"

class ImageDownloader : public cocos2d::Sprite
{
public:

    virtual bool initWithURL(std::string url);
    
    std::string getFileNameFromURL(std::string url);
    bool findFileInLocalCache(std::string fileName);
    void downloadFileFromServer(std::string url);
    void downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    // implement the "static create()" method manually
    CREATE_FUNC(ImageDownloader);
};

#endif // __MAINHUBBGELEMENTS__
