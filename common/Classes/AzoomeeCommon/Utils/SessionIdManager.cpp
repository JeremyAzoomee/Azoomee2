#include "SessionIdManager.h"

using namespace cocos2d;
using namespace Azoomee;

namespace Azoomee
{

    static SessionIdManager *_sharedSessionIdManager = NULL;

    SessionIdManager* SessionIdManager::getInstance()
    {
        if (! _sharedSessionIdManager)
        {
            _sharedSessionIdManager = new SessionIdManager();
            _sharedSessionIdManager->init();
        }
        
        return _sharedSessionIdManager;
    }

    SessionIdManager::~SessionIdManager(void)
    {
    }

    bool SessionIdManager::init(void)
    {
        timeStampGoingBackground = 0;
        sessionId = "";
        generateSessionId();
        return true;
    }

    std::string SessionIdManager::getCurrentSessionId()
    {
        return sessionId;
    }
    
    void SessionIdManager::registerAppWentBackgroundEvent()
    {
        timeStampGoingBackground = time(NULL);
    }
    
    void SessionIdManager::registerAppCameForegroundEvent()
    {
        if(generatingNewSessionIdRequired()) generateSessionId();
    }
    
    //---------------------------------------PRIVATE METHODS--------------------------------------
    
    void SessionIdManager::generateSessionId()
    {
        sessionId = "";
        static const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyz";
       
        for(int i = 0; i < 10; i++) sessionId += alphanum[rand() % (sizeof(alphanum) - 1)];
        
        cocos2d::log("NEW SESSIONID CREATED: %s", sessionId.c_str());
    }
    
    bool SessionIdManager::generatingNewSessionIdRequired()
    {
        if(sessionId == "") return true;
        if(timeStampGoingBackground == 0) return true;
        if(time(NULL) - timeStampGoingBackground > 5) return true;
        
        cocos2d::log("SESSIONID NOT CHANGED: %s", sessionId.c_str());
        
        return false;
    }
    
}
