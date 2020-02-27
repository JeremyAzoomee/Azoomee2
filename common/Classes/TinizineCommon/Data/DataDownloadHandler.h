//
//  DataDownloadHandler.h
//  TinizineCommon
//
//  Created by Macauley on 12/03/2019.
//

#ifndef DataDownloadHandler_h
#define DataDownloadHandler_h

#include "../Tinizine.h"
#include "../API/API.h"
#include <memory>
#include "Json.h"
#include "../Utils/FileDownloader.h"

NS_TZ_BEGIN

class DataDownloadHandler : public FileDownloaderDelegate, public HttpRequestCreatorResponseDelegate
{
protected:
	DataDownloadHandler();
	
	
	typedef std::function<void(bool)> OnCompleteCallback;
	
	FileDownloaderRef _fileDownloader = nullptr;
	OnCompleteCallback _callback = nullptr;
	
	std::string getLocalEtag() const;
	void setLocalEtag(const std::string& etag);
	
	virtual std::string getCachePath() const = 0;
	
	void downloadFile(const std::string& url);
	void sendCallback(bool success);
	
public:
	virtual ~DataDownloadHandler();
	virtual void getLatestData(const OnCompleteCallback& callback = nullptr) = 0;
	
	void setOnCompleteCallback(const OnCompleteCallback& callback);
	
	//delegate functions
	virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
	virtual void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode) override;
};

NS_TZ_END

#endif /* DataDownloadHandler_h */
