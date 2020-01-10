//
//  UserSessionManager.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 09/01/2020.
//

#include "UserSessionManager.h"
#include "../API/API.h"
#include "../Data/Parent/ParentManager.h"
#include "TimeFunctions.h"

NS_AZOOMEE_BEGIN

static std::auto_ptr<UserSessionManager> sUserSessionManagerSharedInstance;

UserSessionManager* UserSessionManager::getInstance()
{
    if(!sUserSessionManagerSharedInstance.get())
    {
        sUserSessionManagerSharedInstance.reset(new UserSessionManager());
    }
    return sUserSessionManagerSharedInstance.get();
}

UserSessionManager::UserSessionManager()
{
}

UserSessionManager::~UserSessionManager()
{
}

void UserSessionManager::refreshUserSession(const OnCompleteCallback& callback)
{
    if(getCurrentTimeMillis() - _lastSessionRefresh > std::chrono::milliseconds(3600000))
    {
        auto onSuccess = [callback, this](const std::string& requestTag, const std::string& headers, const std::string& body) {
            _lastSessionRefresh = getCurrentTimeMillis();
            ParentManager::getInstance()->parseParentSessionData(body);
            if(callback)
            {
                callback(true);
            }
        };
        
        auto onFailure = [callback](const std::string& requestTag, long errorCode) {
            if(callback)
            {
                callback(false);
            }
        };
        
        HttpRequestCreator* request = API::RefreshParentCookiesRequest(onSuccess, onFailure);
        request->execute();
    }
    else
    {
        if(callback)
        {
            callback(false);
        }
    }
}

NS_AZOOMEE_END
