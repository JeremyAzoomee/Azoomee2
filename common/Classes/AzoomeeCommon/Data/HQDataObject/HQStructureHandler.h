//
//  HQStructureHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef HQStructureHandler_h
#define HQStructureHandler_h

#include "../../Azoomee.h"
#include "../../Utils/FileZipUtil.h"
#include "../DataDownloadHandler.h"

NS_AZOOMEE_BEGIN

class HQStructureHandler : public DataDownloadHandler, FileZipDelegate
{
private:
    static const std::string kCachePath;
    static const std::string kZipName;
    
    std::string _feedPath;
    
    void loadLocalData();
	std::string getCachePath() const override;
    
    void parseNavigationData(const std::string& data);
    
public:
    static HQStructureHandler* getInstance();
    ~HQStructureHandler();
    
    void getLatestData(const OnCompleteCallback& callback = nullptr) override;
    
    void loadGroupHQData(const std::string& groupIdPath);
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    virtual void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath) override;
    virtual void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode) override;
};

NS_AZOOMEE_END

#endif /* HQStructureHandler_h */
