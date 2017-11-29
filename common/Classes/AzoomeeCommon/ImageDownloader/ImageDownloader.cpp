#include "ImageDownloader.h"
#include "../Data/Cookie/CookieDataProvider.h"
#include "../Data/ConfigStorage.h"
#include "../Data/Parent/ParentDataProvider.h"

using namespace cocos2d;
using namespace network;


NS_AZOOMEE_BEGIN

#pragma mark - Public

ImageDownloaderRef ImageDownloader::create(const std::string& storageLocation, CacheMode mode)
{
    ImageDownloaderRef downloader(new(std::nothrow) ImageDownloader(storageLocation, mode));
    return downloader;
}

ImageDownloader::ImageDownloader(const std::string& storageLocation, CacheMode mode) :
    _cacheMode(mode)
{
    fileUtils = FileUtils::getInstance();
    _storagePath = fileUtils->getWritablePath() + storageLocation;
    
    // Make sure storageLocation ends with a slash
    if(_storagePath.back() != '/')
    {
        _storagePath += "/";
    }
}

ImageDownloader::~ImageDownloader()
{
    _delegate = nullptr;
    if(_downloadRequest)
    {
        _downloadRequest->setResponseCallback(nullptr);
        _downloadRequest->release();
        _downloadRequest = nullptr;
    }
}

void ImageDownloader::downloadImage(ImageDownloaderDelegate* delegate, const std::string& url)
{
    _delegate = delegate;
    _filename = getFileNameFromURL(url);
    _category = getCategoryFromUrl(url);
    
    bool localFileExists = localImageExists();
    if(localFileExists && !hasCacheExpired())
    {
        loadFileFromLocalCacheAsync();
    }
    else
    {
        if(_cacheMode == CacheMode::Category)
        {
            if(FileUtils::getInstance()->isDirectoryExist(getCategoryPath()))
            {
                fileUtils->removeDirectory(getCategoryPath());
            }
        }
        else
        {
            if(localFileExists)
            {
                fileUtils->removeFile(getLocalImagePath());
            }
        }
        downloadFileFromServer(url);
    }
}

std::string ImageDownloader::getLocalImagePath() const
{
    return getCategoryPath() + _filename;
}

bool ImageDownloader::localImageExists() const
{
    return FileUtils::getInstance()->isFileExist(getLocalImagePath());
}

void ImageDownloader::setDelegate(ImageDownloaderDelegate* delegate)
{
    _delegate = delegate;
}

void ImageDownloader::setCacheMode(ImageDownloader::CacheMode mode)
{
    _cacheMode = mode;
}

#pragma mark - Private

bool ImageDownloader::hasCacheExpired() const
{
    return !ImageDownloader::checkTimeStampValid(getTimestampFilePath());
}

void ImageDownloader::downloadFileFromServer(const std::string& url)
{
    if(url == "")
    {
        return;
    }
    
    _downloadRequest = new HttpRequest();
    _downloadRequest->setRequestType(HttpRequest::Type::GET);
    _downloadRequest->setUrl(url.c_str());
    
    std::vector<std::string> headers{
        "Cookie: " + CookieDataProvider::getInstance()->getCookiesForRequest(url),
        "X-AZ-COUNTRYCODE: " + ParentDataProvider::getInstance()->getLoggedInParentCountryCode()
    };
    _downloadRequest->setHeaders(headers);
    
    _downloadRequest->setResponseCallback(CC_CALLBACK_2(ImageDownloader::downloadFileFromServerAnswerReceived, this));
    _downloadRequest->setTag("image download");
    HttpClient::getInstance()->send(_downloadRequest);
}

void ImageDownloader::downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response)
{
    if(response->getResponseCode() == 200)
    {
        if(response && response->getResponseData())
        {
            std::vector<char> myResponse = *response->getResponseData();
            const std::string& responseString = std::string(myResponse.begin(), myResponse.end());
            
            if(saveFileToDevice(responseString, getLocalImagePath()))
            {
                saveFileToDevice(StringUtils::format("%ld", time(NULL)), getTimestampFilePath());
                loadFileFromLocalCacheAsync();
                return;
            }
            else
            {
                // TODO: Failed callback
                return;
            }
        }
    }
    else
    {
        // TODO: Failed callback
    }
}

void ImageDownloader::loadFileFromLocalCacheAsync()
{
    if(_delegate)
    {
        _delegate->onImageDownloadComplete(shared_from_this());
    }
}

std::string ImageDownloader::getTimestampFilePath() const
{
    // The timestamp path changes depending on the cache mode
    if(_cacheMode == CacheMode::Category)
    {
        // One timestamp file for the category
        return getCategoryPath() + "time.stmp";
    }
    else
    {
        // A timestamp file specific to the file
        return getCategoryPath() + _filename + ".time.stmp";
    }
}

std::string ImageDownloader::getCategoryPath() const
{
    return _storagePath + _category + "/";
}

#pragma mark - Static

std::string ImageDownloader::getFileNameFromURL(const std::string& url)
{
    const uint64_t startPoint = url.find_last_of("/") + 1;
    
    uint64_t endPoint = url.length();
    if(url.find("?", 0) != url.npos) endPoint = url.find("?", 0);
    const uint64_t subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

std::string ImageDownloader::getCategoryFromUrl(const std::string& url)
{
    const uint64_t endPoint = url.find_last_of("/");
    const std::string& cutUrl = url.substr(0, endPoint);
    
    const uint64_t startPoint = cutUrl.find_last_of("/") + 1;
    const std::string& result = cutUrl.substr(startPoint);
    
    return result;
}

bool ImageDownloader::checkTimeStampValid(const std::string& timeStampFilePath)
{
    if(!FileUtils::getInstance()->isFileExist(timeStampFilePath)) return false;
    
    const time_t timeStamp = atoll(FileUtils::getInstance()->getStringFromFile(timeStampFilePath).c_str());
    const time_t currentTimeStamp = time(NULL);
    
    if(currentTimeStamp - timeStamp > ConfigStorage::getInstance()->getContentItemImageValidityInSeconds()) return false;
    
    return true;
}

bool ImageDownloader::saveFileToDevice(const std::string& data, const std::string& fullPath)
{
    cocos2d::FileUtils* fileUtils = FileUtils::getInstance();
    
    // Make sure directories created
    const uint64_t lastSlashIndex = fullPath.find_last_of("/");
    const std::string& parentDirPath = fullPath.substr(0, lastSlashIndex + 1);
    if(!fileUtils->isDirectoryExist(parentDirPath))
    {
        fileUtils->createDirectory(parentDirPath);
    }
    
    // Remove old file
    if(fileUtils->isFileExist(fullPath))
    {
        fileUtils->removeFile(fullPath);
    }
    // Create new file
    if(fileUtils->writeStringToFile(data, fullPath)) return true;
    else return false;
}
  
NS_AZOOMEE_END
