#ifndef AzoomeeCommon_ConfigStorage_h
#define AzoomeeCommon_ConfigStorage_h

//General configuration
//#define FORCE_RELOAD YES;
//#define FORGET_USER_DATA YES;
//#define ALLOW_UNPAID_SIGNUP YES

#include <cocos/cocos2d.h>
#include "Json.h"

namespace Azoomee
{

class ConfigStorage : public cocos2d::Ref
{
    
public:
    
    static const char* const kGroupHQName;
    
	static const char* const kIosWebviewName;
	static const char* const kAndroidWebviewName;
    
    static const std::string kArtCacheFolder;
	static const std::string kOomeeMakerCacheFolder;
    
    static const std::string kAvatarImageCacheFolder;
    
    
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    virtual ~ConfigStorage();
    bool init(void);
    
    
private:
    rapidjson::Document parseJsonConfigurationFile(const std::string& fileName);
    
    rapidjson::Document VersionConfiguration;
    
};
  
}

#endif
