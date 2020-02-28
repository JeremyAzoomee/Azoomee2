#ifndef AzoomeeChat_ChatConfig_h
#define AzoomeeChat_ChatConfig_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <map>
#include <TinizineCommon/Tinizine.h>

USING_NS_TZ

NS_AZ_CHAT_BEGIN

class ChatConfig
{
private:
    
    
    /// Initialise ChatConfig
    void init();
    
public:
    
    /// Get shared instance of ChatConfig
    static ChatConfig* getInstance();
    virtual ~ChatConfig();
    
        
};
  
NS_AZ_CHAT_END

#endif
