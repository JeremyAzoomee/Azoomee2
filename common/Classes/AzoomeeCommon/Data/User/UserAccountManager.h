//
//  UserAccountManager.h
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 20/01/2020.
//

#ifndef UserAccountManager_h
#define UserAccountManager_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "../../Azoomee.h"
#include "../../API/API.h"

NS_AZOOMEE_BEGIN

class UserAccountManager
{
    typedef std::function<void(bool, long)> OnCompleteCallback;
private:
    
public:
    
    static UserAccountManager* getInstance();
    virtual ~UserAccountManager();
    
    bool localLogin();
    void login(const std::string& email, const std::string& password, const OnCompleteCallback& callback);
    void AnonLogin(const OnCompleteCallback& callback);
    
    void getChildrenForLoggedInParent(const OnCompleteCallback& callback);
    
    void getBillingDataForLoggedInParent(const OnCompleteCallback& callback);
    
    void loginChild(const std::string& profileName, const OnCompleteCallback& callback);
    
    void logout();
    
};

NS_AZOOMEE_END

#endif /* UserAccountManager_h */
