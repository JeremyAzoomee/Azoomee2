//
//  OomeeMakerDataHandler.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 15/02/2018.
//

#ifndef OomeeMakerDataHandler_h
#define OomeeMakerDataHandler_h

#include "../AzoomeeOomeeMaker.h"
#include "OomeeMakerDataStorage.h"
#include "OomeeFigureData.h"
#include <AzoomeeCommon/Utils/FileDownloader.h>
#include <AzoomeeCommon/Utils/FileZipUtil.h>
#include <AzoomeeCommon/Data/DataDownloadHandler.h>

NS_AZOOMEE_OM_BEGIN

class OomeeMakerDataHandler : public DataDownloadHandler, public FileZipDelegate
{
private:
    const std::string kBaseFolderName = "oomeeMaker/";
	
    OomeeMakerDataStorage* _dataStorage = nullptr;
	
	std::string getCachePath() const override;
	
    void unzipBundledAssets();
    void removeExistingAssets();
	
	void loadLocalData();
	
    void parseOomeeData();
    void parseCategoryData();
    void parseOomeeItemData();
	
	void updateExistingOomeeFilesToNewIds();
	
	void writeOomeeFiles(const rapidjson::Value& data);
    
public:
    static OomeeMakerDataHandler* getInstance();
    virtual ~OomeeMakerDataHandler();
    void init();
    
    void getConfigFilesIfNeeded();
	
	void getLatestData(const OnCompleteCallback& callback = nullptr) override;
	
	void getOomeesForChild(const std::string& childId, const OnCompleteCallback& callback = nullptr);
	void getAllOomees(const OnCompleteCallback& callback = nullptr);
	
	void saveOomee(const OomeeFigureDataRef& oomee, bool setAsAvatar, const std::string& childId, const OnCompleteCallback& callback = nullptr);
	
    std::string getFullSaveDir() const;
    std::string getLocalSaveDir() const;
    std::string getAssetDir() const;
    
    bool deleteOomee(const std::string& oomeeName );
    
    // Delegate functions
    void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath) override;
    void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode) override;
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
};

NS_AZOOMEE_OM_END

#endif /* OomeeMakerDataHandler_h */
