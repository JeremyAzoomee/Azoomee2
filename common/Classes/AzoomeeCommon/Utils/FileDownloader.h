//
//  FileDownloader.h
//  AzoomeeCommon
//
//  Created by Macauley on 15/12/2017.
//

#ifndef FileDownloader_h
#define FileDownloader_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>
#include <memory>

NS_AZOOMEE_BEGIN

class FileDownloader;
typedef std::shared_ptr<FileDownloader> FileDownloaderRef;

struct FileDownloaderDelegate
{
    virtual void onFileDownloadComplete(const std::string& fileString, int responseCode) = 0;
};

class FileDownloader : public std::enable_shared_from_this<FileDownloader>
{
private:
    FileDownloaderDelegate* _delegate = nullptr;
    std::string _url;
    
public:
    static FileDownloaderRef create(const std::string& url);
    virtual ~FileDownloader();
    /// Set delegate
    void setDelegate(ImageDownloaderDelegate* delegate);
    
    void downloadFileFromServer(const std::string& url);
    void downloadFileFromServerAnswerReceived(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);
};

NS_AZOOMEE_END

#endif /* FileDownloader_h */
