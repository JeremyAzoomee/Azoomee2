#include "ImageDownloader.h"
#include "../Data/ConfigStorage.h"

using namespace cocos2d;
using namespace network;


NS_AZOOMEE_BEGIN

#pragma mark - Public

const int ImageDownloader::kContentImageValidityDurationSecs = 604800;

std::vector<ImageDownloaderRef> ImageDownloader::_downloadingImagePool = std::vector<ImageDownloaderRef>();

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
}

void ImageDownloader::downloadImage(ImageDownloaderDelegate* delegate, const std::string& url, bool forceOverride)
{
    _downloadingImagePool.push_back(shared_from_this());
    _delegate = delegate;
    _filename = getFileNameFromURL(url);
    _category = getCategoryFromUrl(url);
	_url = url;
    bool localFileExists = localImageExists();
    if(localFileExists && !hasCacheExpired() && !forceOverride)
    {
        Director::getInstance()->getTextureCache()->addImageAsync(getLocalImagePath(), [this](Texture2D* texture){
            if(texture)
            {
                this->loadFileFromLocalCacheAsync();
            }
            else
            {
                this->downloadFailed();
            }
        });
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

std::string ImageDownloader::getUrl() const
{
	return _url;
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
    
    _fileDownloader = FileDownloader::create();
    _fileDownloader->setDelegate(this);
    _fileDownloader->downloadFileFromServer(url);
}

void ImageDownloader::loadFileFromLocalCacheAsync()
{
    if(_delegate)
    {
        _delegate->onImageDownloadComplete(shared_from_this());
    }
    _downloadingImagePool.erase(std::find(_downloadingImagePool.begin(), _downloadingImagePool.end(), shared_from_this()));
}

void ImageDownloader::downloadFailed()
{
    if(_delegate)
    {
        _delegate->onImageDownloadFailed();
    }
    _downloadingImagePool.erase(std::find(_downloadingImagePool.begin(), _downloadingImagePool.end(), shared_from_this()));
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
    if(!FileUtils::getInstance()->isFileExist(timeStampFilePath))
    {
        return false;
    }
    
    const time_t timeStamp = atoll(FileUtils::getInstance()->getStringFromFile(timeStampFilePath).c_str());
    const time_t currentTimeStamp = time(NULL);
    
    if(currentTimeStamp - timeStamp > kContentImageValidityDurationSecs)
    {
        return false;
    }
    
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

void ImageDownloader::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
    if(responseCode == 200)
    {
        if(saveFileToDevice(fileString, getLocalImagePath()))
        {
            saveFileToDevice(StringUtils::format("%ld", time(NULL)), getTimestampFilePath());
            Director::getInstance()->getTextureCache()->addImageAsync(getLocalImagePath(), [this](Texture2D* texture){
                if(texture)
                {
                    this->loadFileFromLocalCacheAsync();
                }
                else
                {
                    this->downloadFailed();
                }
            });
        }
        else
        {
            downloadFailed();
        }
    }
    else
    {
        downloadFailed();
    }
}
  
NS_AZOOMEE_END
