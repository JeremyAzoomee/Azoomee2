#ifndef __IMAGEDOWNLOADER_LOGIC_H__
#define __IMAGEDOWNLOADER_LOGIC_H__

#include "cocos2d.h"
#include "network/HttpClient.h"

class ImageDownloaderLogic : public cocos2d::Ref
{
public:
    void startProcessingImage(cocos2d::Node *sender, std::string url);
    
    std::string fileName;
    std::string imageId;
    std::string imageCachePath;
    std::string imageIdPath;
    cocos2d::Node *senderNode;
    bool groupLogo;
    bool senderDeleted;
    
private:
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
    
    cocos2d::network::HttpRequest *downloadRequest;
    cocos2d::FileUtils *fileUtils;
};

#endif
