#ifndef AzoomeeCommon_ImageDownloader_h
#define AzoomeeCommon_ImageDownloader_h

#include "../Tinizine.h"
#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>
#include "../Utils/FileDownloader.h"
#include <memory>


NS_TZ_BEGIN
    
class ImageDownloader;
typedef std::shared_ptr<ImageDownloader> ImageDownloaderRef;

struct ImageDownloaderDelegate
{
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) = 0;
    virtual void onImageDownloadFailed() = 0;
};

class ImageDownloader : public std::enable_shared_from_this<ImageDownloader>, FileDownloaderDelegate
{
public:
    static const std::string kImageCachePath;
    static const std::string kAvatarImageCacheFolder;
    
    enum CacheMode {
        /// A category as a whole is cached together.
        /// When it expires the whole category is deleted
        Category = 0,
        /// A file is cached on it's own, when it expires only the single file is deleted.
        File
    };
    
private:
    static std::vector<ImageDownloaderRef> _downloadingImagePool;
    
    static const int kContentImageValidityDurationSecs;
    
    /// Filename for the image
    std::string _filename;
    /// Category of the image
    std::string _category;
    /// Base Storage path
    std::string _storagePath;
	/// target image url
	std::string _url;
    /// Delegate to recieve callbacks on download
    ImageDownloaderDelegate* _delegate = nullptr;
    /// The current cache mode
    CacheMode _cacheMode = CacheMode::Category;
    /// The request to download the image
    cocos2d::network::HttpRequest* _downloadRequest = nullptr;
    /// Cached FileUtils::getInstance()
    cocos2d::FileUtils* fileUtils;
    
    FileDownloaderRef _fileDownloader = nullptr;
    
    /// Private construction - use ::create
    ImageDownloader(const std::string& storageLocation, CacheMode mode);
    
    /// Get filename from URL
    static std::string getFileNameFromURL(const std::string& url);
    /// Get category from URL
    static std::string getCategoryFromUrl(const std::string& url);
    /// Returns true if timestamp is valid
    static bool checkTimeStampValid(const std::string& timeStampFilePath);
    /// Save a file to local device
    static bool saveFileToDevice(const std::string& data, const std::string& fileName);
    
    /// Returns true if cached image expired
    bool hasCacheExpired() const;
    /// Return the absolute path to the category
    std::string getCategoryPath() const;
    /// Return location of timestamp file
    std::string getTimestampFilePath() const;
    
    void downloadFileFromServer(const std::string& url);
    void loadFileFromLocalCacheAsync();
    void downloadFailed();
    
public:
    static ImageDownloaderRef create(const std::string& storageLocation, CacheMode mode);
    virtual ~ImageDownloader();
    
    /// Set the cache mode
    /// The default is CacheMode::Category
    void setCacheMode(CacheMode mode);
    /// Set delegate
    void setDelegate(ImageDownloaderDelegate* delegate);
    
    /// Returns the path to the local image file
    std::string getLocalImagePath() const;
    /// Returns true if the local image exists
    bool localImageExists() const;
    /// Returns url image is/was requested from
	std::string getUrl() const;
    /// Download or load the image from local cache
    void downloadImage(ImageDownloaderDelegate* delegate, const std::string& url, bool forceOverride = false);
    
    // Delegate functions
    void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
};
  
NS_TZ_END

#endif
