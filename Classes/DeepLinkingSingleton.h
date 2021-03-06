#ifndef __DEEP_LINKING_SINGLETON_H__
#define __DEEP_LINKING_SINGLETON_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class DeepLinkingSingleton : public cocos2d::Ref, public Azoomee::MessageBoxDelegate
{
private:
    std::string host;
    std::string path;
    
    //Used to avoid loop of childLogin, if content ID is not valid.
    bool deepLinkActionWaiting;
    
    bool setHostAndPath(const std::string& uriString);
    
    void completeContentAction(const HQContentItemObjectRef &contentItem);
    
    void moveToHQ(const std::string& hqName);
    void resetDeepLink();
    std::string getDataForKeyFromJSON(const std::string& jsonString, const std::string& key);
    
public:
    static DeepLinkingSingleton* getInstance(void);
    virtual ~DeepLinkingSingleton();
    bool init(void);
    
    void setDeepLink(const std::string& uriString);
    bool actionDeepLink();
    void contentDetailsResponse(const std::string& responseBody);
    
    //Delegate Functions
    void MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle);
    
    static const std::string kPostContentDeeplinkStr;
};

NS_AZOOMEE_END

#endif
