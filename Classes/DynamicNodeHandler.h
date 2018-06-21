//
//  DynamicNodeHandler.h
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#ifndef DynamicNodeHandler_h
#define DynamicNodeHandler_h

#include <AzoomeeCommon/Azoomee.h>
#include "cocos2d.h"
#include "DynamicNodeCreator.h"
#include "DynamicNodeFlowController.h"
#include "IAPFlowController.h"
#include <AzoomeeCommon/Data/Json.h>
#include "network/HttpClient.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Utils/FileZipUtil.h>
#include <AzoomeeCommon/Utils/FileDownloader.h>

NS_AZOOMEE_BEGIN

class DynamicNodeHandler : public cocos2d::Ref, public FileDownloaderDelegate, public FileZipDelegate
{
private:
    FileDownloaderRef _fileDownloader = nullptr;
    
    DynamicNodeFlowControllerRef _flowController = nullptr;
    
    bool isCTAPackageJSONExist();
    
    rapidjson::Document getLocalCTAPackageJSON();
    
    void checkIfVersionChangedFromLastCTAPull();
    
    void getCTAPackageJSON(const std::string& url);
    
    void getCTAPackageZip(const std::string& url);
    
    bool unzipCTAFiles(const char *zipPath,const char *dirpath,const char *passwd);
    bool unzipBundleCTAFiles();
    void removeCTAFiles();
    
    std::string getPackageJsonLocation() const;
    std::string getCTADirectoryPath() const;
    std::string getLastPullAppVersionFilePath() const;
    std::string getBundledAssetsPath() const;
    
    void createDynamicNodeFromFile(const std::string& file);
    void createDynamicNodeFromFileWithParams(const std::string &file, const std::string& params);
    
    void jsonDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
    void zipDownloadComplte(const std::string& fileString, const std::string& tag, long responseCode);
    
    // file download request tags
    const std::string _kZipTag = "zip";
    const std::string _kJsonTag = "json";
    
public:
    //-----start popup group names here
    static const std::string kUpgradeGroup;
    
    //-----end popup group names
    
    std::string _currentCTAFile;
    bool _currentCTAUsingParams = false;
    std::string _currentCTAParams;
    
    static DynamicNodeHandler* getInstance(void);
    virtual ~DynamicNodeHandler();
    bool init(void);
    
    void createDynamicNodeById(const std::string& uniqueId);
    void createDynamicNodeByGroupId(const std::string& groupId);
    
    void createDynamicNodeByIdWithParams(const std::string& uniqueId, const std::string& params);
    void createDynamicNodeByGroupIdWithParams(const std::string& group, const std::string& params);
    
    void startSignupFlow();
    void startIAPFlow(IAPEntryContext context = DEFAULT);
    void startAddChildFlow();
    void handleSuccessFailEvent();
    
    void getCTAFiles();
    
    void setFlowController(DynamicNodeFlowControllerRef flowController);
    DynamicNodeFlowControllerRef getFlowController();
    
    void rebuildCurrentCTA();
    
    // Delegate functions
    void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath);
    void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
    
};

NS_AZOOMEE_END

#endif /* DynamicNodeHandler_h */
