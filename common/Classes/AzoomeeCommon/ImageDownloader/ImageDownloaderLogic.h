#ifndef AzoomeeCommon_ImageDownloaderLogic_h
#define AzoomeeCommon_ImageDownloaderLogic_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>


NS_AZOOMEE_BEGIN
    
class ImageDownloaderLogic;

struct ImageDownloaderDelegate
{
    virtual void onImageDownloadComplete(ImageDownloaderLogic* downloader) = 0;
};

class ImageDownloaderLogic : public cocos2d::Ref
{
private:
    
    /// Filename for the image
    std::string _filename;
    /// Image ID
    std::string _imageId;
    /// Delegate to recieve callbacks on download
    ImageDownloaderDelegate* _delegate = nullptr;
    bool groupLogo;
    bool senderDeleted;
    /// The request to download the image
    cocos2d::network::HttpRequest* _downloadRequest = nullptr;
    /// Cached FileUtils::getInstance()
    cocos2d::FileUtils* fileUtils;
    
    
    /// Get filename from URL
    static std::string getFileNameFromURL(const std::string& url);
    /// Get image Id from URL
    static std::string getIdFromUrl(const std::string& url);
    /// Returns true if imageId needs update
    static bool imageUpdateRequired(const std::string& imageIdPath);
    
    /// Returns true if cached image expired
    bool hasCacheExpired() const;
    std::string getImageIdPath() const;
    
    void downloadFileFromServer(const std::string& url);
    void downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);
    void loadFileFromLocalCacheAsync();
    /// Save a file to local device
    bool saveFileToDevice(const std::string& data, const std::string& fileName);
    
protected:
    
    virtual bool init();
    
    /// Return absolute path to which directory to store the cached image
    virtual std::string getImageCachePath() const;
    
public:
    CREATE_FUNC(ImageDownloaderLogic);
    virtual ~ImageDownloaderLogic();
    
    /// Set delegate
    void setDelegate(ImageDownloaderDelegate* delegate);
    
    /// Returns the path to the local image file
    std::string getLocalImagePath() const;
    
    /// Returns true if the local image exists
    bool localImageExists() const;
    
    /// Download or load the image from local cache
    void downloadImage(ImageDownloaderDelegate* delegate, const std::string& url);
};
  
NS_AZOOMEE_END

#endif
