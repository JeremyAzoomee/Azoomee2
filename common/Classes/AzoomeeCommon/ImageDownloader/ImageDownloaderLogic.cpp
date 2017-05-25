#include "ImageDownloaderLogic.h"
#include "../Data/Cookie/CookieDataProvider.h"
#include "ImageDownloader.h"
#include "../Data/ConfigStorage.h"

using namespace cocos2d;
using namespace network;


NS_AZOOMEE_BEGIN

#pragma mark - Public

ImageDownloaderLogic::~ImageDownloaderLogic()
{
    _delegate = nullptr;
    if(_downloadRequest)
    {
        _downloadRequest->setResponseCallback(nullptr);
        _downloadRequest = nullptr;
    }
}

bool ImageDownloaderLogic::init()
{
    fileUtils = FileUtils::getInstance();
    return true;
}

void ImageDownloaderLogic::downloadImage(ImageDownloaderDelegate* delegate, const std::string& url)
{
    _delegate = delegate;
    _filename = getFileNameFromURL(url);
    _imageId = getIdFromUrl(url);
    
    bool localFileExists = localImageExists();
    if(localFileExists && !hasCacheExpired())
    {
        loadFileFromLocalCacheAsync();
    }
    else
    {
        if(localFileExists)
        {
            const std::string& imageIdPath = getImageIdPath();
            fileUtils->removeDirectory(imageIdPath);
        }
        downloadFileFromServer(url);
    }
}

std::string ImageDownloaderLogic::getLocalImagePath() const
{
    return getImageIdPath() + _filename;
}

bool ImageDownloaderLogic::localImageExists() const
{
    return FileUtils::getInstance()->isFileExist(getLocalImagePath());
}

void ImageDownloaderLogic::setDelegate(ImageDownloaderDelegate* delegate)
{
    _delegate = delegate;
}

#pragma mark - Private

bool ImageDownloaderLogic::hasCacheExpired() const
{
    return ImageDownloaderLogic::imageUpdateRequired(getImageIdPath());
}

void ImageDownloaderLogic::downloadFileFromServer(const std::string& url)
{
    _downloadRequest = new HttpRequest();
    _downloadRequest->setRequestType(HttpRequest::Type::GET);
    _downloadRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", CookieDataProvider::getInstance()->getCookiesForRequest(url).c_str()));
    _downloadRequest->setHeaders(headers);
    
    _downloadRequest->setResponseCallback(CC_CALLBACK_2(ImageDownloaderLogic::downloadFileFromServerAnswerReceived, this));
    _downloadRequest->setTag("image download");
    HttpClient::getInstance()->send(_downloadRequest);
}

void ImageDownloaderLogic::downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response)
{
    if(response->getResponseCode() == 200)
    {
        if(response && response->getResponseData())
        {
            std::vector<char> myResponse = *response->getResponseData();
            const std::string& responseString = std::string(myResponse.begin(), myResponse.end());
            
            if(saveFileToDevice(responseString, _filename))
            {
                saveFileToDevice(StringUtils::format("%ld", time(NULL)), "time.stmp");
                loadFileFromLocalCacheAsync();
                _downloadRequest->release();
                _downloadRequest = nullptr;
                return;
            }
            else
            {
                // TODO: Failed callback
                _downloadRequest->release();
                _downloadRequest = nullptr;
                return;
            }
        }
    }
    else
    {
        // TODO: Failed callback
    }
}

bool ImageDownloaderLogic::saveFileToDevice(const std::string& data, const std::string& fileName)
{
    const std::string& imageCachePath = getImageCachePath();
    if(!fileUtils->isDirectoryExist(imageCachePath))
    {
        fileUtils->createDirectory(imageCachePath);
    }
    const std::string& imageIdPath = getImageIdPath();
    if(!fileUtils->isDirectoryExist(imageIdPath))
    {
        fileUtils->createDirectory(imageIdPath);
    }
    
    const std::string& fullPath = imageIdPath + fileName;
    if(fileUtils->isFileExist(fullPath))
    {
        fileUtils->removeFile(fullPath);
    }
    
    if(fileUtils->writeStringToFile(data, fullPath)) return true;
    else return false;
}

void ImageDownloaderLogic::loadFileFromLocalCacheAsync()
{
    if(_delegate)
    {
        _delegate->onImageDownloadComplete(this);
    }
}

std::string ImageDownloaderLogic::getImageIdPath() const
{
    return getImageCachePath() + _imageId + "/";
}

std::string ImageDownloaderLogic::getImageCachePath() const
{
    return fileUtils->getWritablePath() + "imageCache/";
}

#pragma mark - Static

std::string ImageDownloaderLogic::getFileNameFromURL(const std::string& url)
{
    int startPoint = (int)url.find_last_of("/") + 1;
    
    int endPoint = (int)url.length();
    if(url.find("?", 0) != url.npos) endPoint = (int)url.find("?", 0);
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

std::string ImageDownloaderLogic::getIdFromUrl(const std::string& url)
{
    int endPoint = (int)url.find_last_of("/");
    std::string cutUrl = url.substr(0, endPoint);
    
    int startPoint = (int)cutUrl.find_last_of("/") + 1;
    std::string result = cutUrl.substr(startPoint);
    
    CCLOG("image id: %s", result.c_str());
    
    return result;
}

bool ImageDownloaderLogic::imageUpdateRequired(const std::string& imageIdPath)
{
    std::string timeStampFilePath = imageIdPath + "time.stmp";
    if(!FileUtils::getInstance()->isFileExist(timeStampFilePath)) return true;
    
    long timeStamp = std::atoi(FileUtils::getInstance()->getStringFromFile(timeStampFilePath).c_str());
    long currentTimeStamp = time(NULL);
    
    if(currentTimeStamp - timeStamp > ConfigStorage::getInstance()->getContentItemImageValidityInSeconds()) return true;
    
    return false;
}
  
NS_AZOOMEE_END
