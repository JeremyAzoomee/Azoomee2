//
//  ContentItemPoolHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef ContentItemPoolHandler_h
#define ContentItemPoolHandler_h

#include "../../Azoomee.h"
#include "../../API/HttpRequestCreator.h"
#include "../../Utils/FileZipUtil.h"
#include "../../Utils//FileDownloader.h"

NS_AZOOMEE_BEGIN

class ContentPoolDelegate
{
public:
    virtual void onContentDownloadComplete() = 0;
};


class ContentItemPoolHandler : public HttpRequestCreatorResponseDelegate, FileZipDelegate, FileDownloaderDelegate
{
private:
    static const std::string kCachePath;
    
    FileDownloaderRef _fileDownloader = nullptr;
    ContentPoolDelegate* _delegate = nullptr;
    
    std::string getLocalEtag() const;
    void setLocalEtag(const std::string& etag);
    
    void loadLocalData();
public:
    static ContentItemPoolHandler* getInstance();
    ~ContentItemPoolHandler();
    
    void setContentPoolDelegate(ContentPoolDelegate* delegate);
    
    void getLatestContentPool();
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode);
    virtual void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath);
    virtual void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
};

NS_AZOOMEE_END

#endif /* ContentItemPoolHandler_h */
