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

NS_AZOOMEE_BEGIN

class DynamicNodeHandler : cocos2d::Ref
{
private:
    std::vector<std::string> getFilesInDirectory(const std::string& path);
    std::vector<std::string> getFoldersInDirectory(const std::string& path);
    
    bool isCTAPackageJSONExist();
    
    rapidjson::Document getLocalCTAPackageJSON();
    
    void getCTAPackageJSON(std::string url);
    void onGetCTAPackageJSONAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getCTAPackageZip(std::string url);
    void onGetCTAPackageZipAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool unzipCTAFiles(const char *zipPath,const char *dirpath,const char *passwd);
    bool removeCTAFiles();
    
    std::string getPackageJsonLocation();
    
public:
    static DynamicNodeHandler* getInstance(void);
    virtual ~DynamicNodeHandler();
    bool init(void);
    
    void createDynamicNodeById(const std::string& uniqueId);
    void createDynamicNodeByGroupId(const std::string& groupId);
    
    void getCTAFiles();
    
};

NS_AZOOMEE_END

#endif /* DynamicNodeHandler_h */
