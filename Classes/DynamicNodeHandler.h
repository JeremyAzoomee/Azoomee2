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
#include <AzoomeeCommon/Data/Json.h>
#include "network/HttpClient.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class DynamicNodeHandler : cocos2d::Ref
{
private:
    
    bool isCTAPackageJSONExist();
    
    rapidjson::Document getLocalCTAPackageJSON();
    
    void checkIfVersionChangedFromLastCTAPull();
    
    void getCTAPackageJSON(const std::string& url);
    void onGetCTAPackageJSONAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getCTAPackageZip(const std::string& url);
    void onGetCTAPackageZipAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool unzipCTAFiles(const char *zipPath,const char *dirpath,const char *passwd);
    bool unzipBundleCTAFiles();
    void removeCTAFiles();
    
    std::string getPackageJsonLocation() const;
    std::string getCTADirectoryPath() const;
    std::string getLastPullAppVersionFilePath() const;
    std::string getBundledAssetsPath() const;
    
    void createDynamicNodeFromFile(const std::string& file);
    void createDynamicNodeFromFileWithParams(const std::string &file, const std::string& params);
    
public:
    //-----start popup group names here
    static const std::string kUpgradeGroup;
    
    //-----end popup group names
    
    static DynamicNodeHandler* getInstance(void);
    virtual ~DynamicNodeHandler();
    bool init(void);
    
    void createDynamicNodeById(const std::string& uniqueId);
    void createDynamicNodeByGroupId(const std::string& groupId);
    
    void createDynamicNodeByIdWithParams(const std::string& uniqueId, const std::string& params);
    
    void getCTAFiles();
    
};

NS_AZOOMEE_END

#endif /* DynamicNodeHandler_h */
