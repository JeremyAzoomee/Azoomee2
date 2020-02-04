//
//  AppConfig.h
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 03/02/2020.
//

#ifndef AppConfig_h
#define AppConfig_h

#include "../Azoomee.h"
#include "Json.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class AppConfig
{
private:
    
    rapidjson::Document _appConfigData;
    std::string _env = "";
    
public:
    static const std::string kArtCacheFolder;
    
    static AppConfig* getInstance();
    ~AppConfig();
    void init();
    
    //API configuration
    std::string getServerHost();
    std::string getServerUrlPrefix();
    std::string getServerUrl();
    std::string getRemoteWebGameAPIPath();
    
    //Version configuration
    std::string getVersionNumber();
    std::string getVersionNumberWithPlatform();
    std::string getVersionNumberToDisplay();
    std::string getVersionInformationForRequestHeader();
    
    //Data download congiguration
    std::string getLocaleZipUrl();
    
    
};

NS_AZOOMEE_END

#endif /* AppConfig_h */
