#ifndef __DEEP_LINKING_SINGLETON_H__
#define __DEEP_LINKING_SINGLETON_H__

#include "cocos2d.h"
#include <AzoomeeCommon/UI/MessageBox.h>

class DeepLinkingSingleton : public cocos2d::Ref, public Azoomee::MessageBoxDelegate
{
private:
    std::string Host;
    std::string Path;
    
    //Used to avoid loop of childLogin, if content ID is not valid.
    bool deepLinkActionWaiting;
    
    bool setHostandPath(std::string UriString);
    
    void completeContentAction(std::string type,std::string uri);
    void resetDeepLink();
    
public:
    static DeepLinkingSingleton* getInstance(void);
    virtual ~DeepLinkingSingleton();
    bool init(void);
    
    void setDeepLink(std::string UriString);
    bool actionDeepLink();
    void contentDetailsResponse(std::string responseBody);
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
};

#endif
