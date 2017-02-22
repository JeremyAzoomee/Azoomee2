#include "cocos2d.h"
#include "network/HttpClient.h"

class OfflineChecker : public cocos2d::Ref
{
    
public:
    static OfflineChecker* getInstance(void);
    virtual ~OfflineChecker();
    bool init(void);
    
    bool getOfflineStatus();
    void startOfflineChecking();
    
    void setLayerToShowWhenOffline(cocos2d::Node* layer);
    cocos2d::Node* layerToShowWhenOffline;
    
private:
    void sendOfflineCheckRequest();
    void onOfflineCheckRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool offlineStatus;
};
