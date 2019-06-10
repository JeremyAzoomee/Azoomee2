//
//  ContentItemPoolDownloadHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef ContentItemPoolDownloadHandler_h
#define ContentItemPoolDownloadHandler_h

#include "../../Azoomee.h"
#include "../../Utils/FileZipUtil.h"
#include "../DataDownloadHandler.h"

NS_AZOOMEE_BEGIN

class ContentItemPoolDownloadHandler : public DataDownloadHandler, FileZipDelegate
{
private:
    static const std::string kCachePath;
	
	std::string getCachePath() const override;
	
    void loadLocalData();
	
	void parseContentItemPool(const std::string& contentItemPoolString);
	void parseContentItem(const std::string& contentId, const rapidjson::Value& contentItemData);
public:
    static ContentItemPoolDownloadHandler* getInstance();
    ~ContentItemPoolDownloadHandler();
    
	void getLatestData(const OnCompleteCallback& callback = nullptr) override;
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    virtual void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath) override;
    virtual void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode) override;
};

NS_AZOOMEE_END

#endif /* ContentItemPoolDownloadHandler_h */
