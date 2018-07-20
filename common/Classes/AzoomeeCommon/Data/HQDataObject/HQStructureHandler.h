//
//  HQStructureHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef HQStructureHandler_h
#define HQStructureHandler_h

#include "../../Azoomee.h"
#include "../../API/HttpRequestCreator.h"
#include "../../Utils/FileZipUtil.h"
#include "../../Utils/FileDownloader.h"

NS_AZOOMEE_BEGIN
class HQFeedDelegate
{
public:
    virtual void onFeedDownloadComplete() = 0;
};

class HQStructureHandler : public HttpRequestCreatorResponseDelegate, FileZipDelegate, FileDownloaderDelegate
{
private:
    FileDownloaderRef _fileDownloader = nullptr;
    HQFeedDelegate* _delegate = nullptr;
    
    void loadLocalData();
    
public:
    static HQStructureHandler* getInstance();
    ~HQStructureHandler();
    
    void setHQFeedDelegate(HQFeedDelegate* delegate);
    
    void getLatestHQStructureFeed();
    
    void loadGroupHQData(const std::string& groupIdPath);
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode);
    virtual void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath);
    virtual void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
};

NS_AZOOMEE_END

#endif /* HQStructureHandler_h */
