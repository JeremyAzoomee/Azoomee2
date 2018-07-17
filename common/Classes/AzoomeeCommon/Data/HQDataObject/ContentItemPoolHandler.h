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

class ContentItemPoolHandler : public HttpRequestCreatorResponseDelegate, FileZipDelegate, FileDownloaderDelegate
{
private:
    FileDownloaderRef _fileDownloader = nullptr;
    
    bool isNewerVersion(time_t timestamp);
    void loadLocalData();
public:
    static ContentItemPoolHandler* getInstance();
    ~ContentItemPoolHandler();
    
    void getLatestContentPool();
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode);
    virtual void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath);
    virtual void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
};

NS_AZOOMEE_END

#endif /* ContentItemPoolHandler_h */
