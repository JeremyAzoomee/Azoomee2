#ifndef Azoomee_ForceUpdateSingleton_h
#define Azoomee_ForceUpdateSingleton_h

#include <cocos/cocos2d.h>
#include <TinizineCommon/Tinizine.h>
#include <TinizineCommon/Utils/FileDownloader.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

enum class ForceUpdateResult {DO_NOTHING, NOTIFY, LOCK};

class ForceUpdateDelegate
{
public:
	virtual void onForceUpdateCheckFinished(const ForceUpdateResult& result) = 0;
};

class ForceUpdateSingleton : public cocos2d::Ref, FileDownloaderDelegate
{
    
public:
    static ForceUpdateSingleton* getInstance(void);
    virtual ~ForceUpdateSingleton();

    void doForceUpdateLogic();
	
	void setDelegate(ForceUpdateDelegate* delegate);
    
    std::string getUpdateUrlFromFile();
	
	void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode) override;
    
private:
    ForceUpdateSingleton();
    void onForceUpdateLogicHasLocalFile();
	bool remoteForceUpdateDataDownloadRequired();
	bool parseAndSaveForceUpdateData(const std::string &jsonString);
	
	std::string getLocalEtag() const;
	void setLocalEtag(const std::string& etag);
	
    bool isNotificationRequired();
    bool isAppCloseRequired();
    std::string getAcceptedMinAzoomeeVersion();
    std::string getNotifiedMinAzoomeeVersion();
    std::map<std::string, std::string> getMapFromForceUpdateJsonData(const std::string &forceUpdateJsonData);
    void createUpdateDirectory();
    
    std::string writablePath;
	
	FileDownloaderRef _fileDownloader = nullptr;
	ForceUpdateDelegate* _delegate = nullptr;
	
	static const std::string kAcceptedMinAzVerID;
	static const std::string kNotifiedMinAzVerID;
	static const std::string kAcceptedMinAzVerVodaID;
	static const std::string kNotifiedMinAzVerVodaID;
	static const std::string kUpdateUrlAppleID;
	static const std::string kUpdateUrlGoogleID;
	static const std::string kUpdateUrlAmazonID;
	static const std::string kUpdateUrlVodaID;
};

NS_AZ_END

#endif
