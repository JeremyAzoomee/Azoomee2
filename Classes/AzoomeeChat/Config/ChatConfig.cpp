#include "ChatConfig.h"
#include <AzoomeeCommon/Data/Json.h>

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

//const char* const kConfigDir = "res/chat/config/";


static std::auto_ptr<ChatConfig> sChatConfigSharedInstance;

ChatConfig* ChatConfig::getInstance()
{
    if(!sChatConfigSharedInstance.get())
    {
        sChatConfigSharedInstance.reset(new ChatConfig());
        sChatConfigSharedInstance->init();
    }
    return sChatConfigSharedInstance.get();
}

ChatConfig::~ChatConfig()
{
}

void ChatConfig::init()
{
//    std::string path = std::string(kConfigDir) + kConfigPath;
//    CCASSERT(FileUtils::getInstance()->isFileExist(path), "Chat config file does not exist");
//    
//    const std::string& jsonString = FileUtils::getInstance()->getStringFromFile(path);
//    rapidjson::Document json;
//    json.Parse(jsonString.c_str());
}

NS_AZOOMEE_CHAT_END
