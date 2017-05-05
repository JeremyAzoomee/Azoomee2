#include "ImageDownloaderLogic.h"
#include "../Data/Cookie/CookieDataStorage.h"
#include "ImageDownloader.h"
#include "../Data/ConfigStorage.h"

using namespace cocos2d;
using namespace network;


namespace Azoomee
{

void ImageDownloaderLogic::startProcessingImage(cocos2d::Node *sender, std::string url)
{
    fileUtils = FileUtils::getInstance();
    
    fileName = getFileNameFromURL(url);
    imageId = getIdFromUrl(url);
    imageCachePath = getImageCachePath();
    imageIdPath = getImageIdPath();
    senderNode = sender;
    
    if(findFileInLocalCache(fileName))
    {
        if(imageUpdateRequired(fileName))
        {
            fileUtils->removeDirectory(imageIdPath);
            downloadFileFromServer(url);
        }
        else loadFileFromLocalCacheAsync(fileName);
    }
    else
    {
        downloadFileFromServer(url);
    }

}
    
bool ImageDownloaderLogic::imageUpdateRequired(std::string fileName)
{
    std::string timeStampFilePath = imageIdPath + "time.stmp";
    if(!fileUtils->isFileExist(timeStampFilePath)) return true;
    
    long timeStamp = std::atoi(fileUtils->getStringFromFile(timeStampFilePath).c_str());
    long currentTimeStamp = time(NULL);
    
    if(currentTimeStamp - timeStamp > ConfigStorage::getInstance()->getContentItemImageValidityInSeconds()) return true;
    
    return false;
}

std::string ImageDownloaderLogic::getFileNameFromURL(std::string url)
{
    int startPoint = (int)url.find_last_of("/") + 1;
    
    int endPoint = (int)url.length();
    if(url.find("?", 0) != url.npos) endPoint = (int)url.find("?", 0);
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

std::string ImageDownloaderLogic::getIdFromUrl(std::string url)
{
    int endPoint = (int)url.find_last_of("/");
    std::string cutUrl = url.substr(0, endPoint);
    
    int startPoint = (int)cutUrl.find_last_of("/") + 1;
    std::string result = cutUrl.substr(startPoint);
    
    CCLOG("image id: %s", result.c_str());
    
    return result;
}

bool ImageDownloaderLogic::findFileInLocalCache(std::string fileName)
{
    return fileUtils->isFileExist(imageIdPath + fileName);
}

void ImageDownloaderLogic::downloadFileFromServer(std::string url)
{
    downloadRequest = new HttpRequest();
    downloadRequest->setRequestType(HttpRequest::Type::GET);
    downloadRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", CookieDataStorage::getInstance()->dataDownloadCookiesForCpp.c_str()));
    downloadRequest->setHeaders(headers);
    
    downloadRequest->setResponseCallback(CC_CALLBACK_2(ImageDownloaderLogic::downloadFileFromServerAnswerReceived, this));
    downloadRequest->setTag("image download");
    HttpClient::getInstance()->send(downloadRequest);
}

void ImageDownloaderLogic::downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response->getResponseCode() == 200)
    {
        if (response && response->getResponseData())
        {
            std::vector<char> myResponse = *response->getResponseData();
            responseString = std::string(myResponse.begin(), myResponse.end());
            
            std::string fileName = getFileNameFromURL(response->getHttpRequest()->getUrl());
            if(saveFileToDevice(responseString, fileName))
            {
                saveFileToDevice(StringUtils::format("%ld", time(NULL)), "time.stmp");
                loadFileFromLocalCacheAsync(fileName);
                downloadRequest->release();
                return;
            }
            else
            {
                downloadRequest->release();
                return;
            }
        }
    }
}

void ImageDownloaderLogic::removeLoadingAnimation()
{
    if((!senderDeleted)&&(!groupLogo))
    {
        if(senderNode->getChildByName("loadingAnimation"))
        {
            senderNode->removeChild(senderNode->getChildByName("loadingAnimation"), true);
        }
    }
}

bool ImageDownloaderLogic::saveFileToDevice(std::string data, std::string fileName)
{
    if(!fileUtils->isDirectoryExist(imageCachePath))
    {
        fileUtils->createDirectory(imageCachePath);
    }
    
    if(!fileUtils->isDirectoryExist(imageIdPath))
    {
        fileUtils->createDirectory(imageIdPath);
    }
    
    if(fileUtils->writeStringToFile(data, imageIdPath + fileName)) return true;
    else return false;
}

void ImageDownloaderLogic::loadFileFromLocalCacheAsync(std::string fileName) //ADD TO SENDER IF ISRUNNING
{
    removeLoadingAnimation();
    
    if(!senderDeleted)
    {
        ImageDownloader *workingFor = (ImageDownloader *)senderNode;
        workingFor->addDownloadedImage(getImageIdPath() + fileName);
    }
}

std::string ImageDownloaderLogic::getImageIdPath()
{
    return fileUtils->getWritablePath() + "imagecache/" + imageId + "/";
}

std::string ImageDownloaderLogic::getImageCachePath()
{
    return fileUtils->getWritablePath() + "imageCache/";
}
  
}
