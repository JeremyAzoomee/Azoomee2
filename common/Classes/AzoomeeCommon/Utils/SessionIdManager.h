#ifndef AzoomeeCommon_SessionIdManager_h
#define AzoomeeCommon_SessionIdManager_h

#include <cocos/cocos2d.h>

namespace Azoomee
{

class SessionIdManager : public cocos2d::Ref
{
    
public:
    static SessionIdManager* getInstance(void);
    
    virtual ~SessionIdManager();
    bool init(void);
    
    std::string getCurrentSessionId();
    void registerAppWentBackgroundEvent();
    void registerAppCameForegroundEvent();
    
private:
    std::string sessionId;
    long timeStampGoingBackground;
    
    bool generatingNewSessionIdRequired();
    void generateSessionId();
};
  
}

#endif
