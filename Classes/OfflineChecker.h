#ifndef __OFFLINECHECKER_H__
#define __OFFLINECHECKER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "network/HttpClient.h"

NS_AZOOMEE_BEGIN

class OfflineChecker;

class OfflineCheckerDelegate
{
public:
    virtual void connectivityStateChanged(bool online) = 0;
};

class OfflineChecker : public cocos2d::Ref
{
    
public:
    static OfflineChecker* getInstance(void);
    virtual ~OfflineChecker();
    bool init(void);
    
    bool getOfflineStatus();
    void startOfflineChecking();
    
    CC_SYNTHESIZE(OfflineCheckerDelegate*, _delegate, Delegate);
    
    void onOfflineCheckAnswerReceived();
    void onOfflineCheckFailed();
    
private:
    void sendOfflineCheckRequest();
    
    bool _offlineStatus = false;
    bool _newScene = false;
    
    int _amountOfFailures = 0;
};

NS_AZOOMEE_END

#endif
