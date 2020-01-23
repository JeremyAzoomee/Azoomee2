//
//  UserAccountManager.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 20/01/2020.
//

#include "UserAccountManager.h"
#include "../Parent/ParentManager.h"
#include "../ConfigStorage.h"
#include "../../Utils/StringFunctions.h"
#include "../Cookie/CookieManager.h"
#include "../Child/ChildManager.h"
#include "../HQDataObject/ContentItemPoolDownloadHandler.h"
#include "../HQDataObject/HQStructureDownloadHandler.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../../Utils/IosNativeFunctionsSingleton.h"
#endif
NS_AZOOMEE_BEGIN

static std::auto_ptr<UserAccountManager> sUserAccountManagerSharedInstance;

UserAccountManager* UserAccountManager::getInstance()
{
    if(!sUserAccountManagerSharedInstance.get())
    {
        sUserAccountManagerSharedInstance.reset(new UserAccountManager());
    }
    return sUserAccountManagerSharedInstance.get();
}

UserAccountManager::~UserAccountManager()
{
    
}

bool UserAccountManager::localLogin()
{
    if(ParentManager::getInstance()->hasParentLoginDataInUserDefaults())
    {
        ParentManager::getInstance()->retrieveParentLoginDataFromUserDefaults();
        return true;
    }
    return false;
}

void UserAccountManager::login(const std::string& email, const std::string& password, const OnCompleteCallback& callback)
{
     auto onSuccess = [callback](const std::string& tag, const std::string& headers, const std::string& body){
           ParentManager::getInstance()->parseParentLoginData(body);
           if(callback)
           {
               callback(true, 200);
           }
       };
       
       auto onFailed = [callback](const std::string& tag, long errorCode){
           if(callback)
           {
               callback(false, errorCode);
           }
       };
       
       HttpRequestCreator* request = API::LoginRequest(email, password, onSuccess, onFailed);
       request->execute();
}

void UserAccountManager::AnonLogin(const OnCompleteCallback& callback)
{
    cocos2d::UserDefault* userDefault = cocos2d::UserDefault::getInstance();
    const std::string& anonEmail = userDefault->getStringForKey(ConfigStorage::kAnonEmailKey, "");
    
    if(anonEmail == "")
    {
        auto onSuccess = [this, callback](const std::string& tag, const std::string& headers, const std::string& body){
            rapidjson::Document json;
            json.Parse(body.c_str());
            const std::string& userId = getStringFromJson("id", json);
            ParentManager::getInstance()->saveAnonCredentialsToDevice(userId);
            this->login(userId, ConfigStorage::kAnonLoginPW, callback);
        };
        
        auto onFailed = [callback](const std::string& tag, long errorCode){
            if(callback)
            {
                callback(false, errorCode);
            }
        };
        HttpRequestCreator* request = API::GetAnonCredentials(onSuccess, onFailed);
        request->execute();
    }
    else
    {
        login(anonEmail, ConfigStorage::kAnonLoginPW, callback);
    }
}

void UserAccountManager::getBillingDataForLoggedInParent(const OnCompleteCallback& callback)
{
    auto onSuccess = [callback](const std::string& tag, const std::string& headers, const std::string& body){
        ParentManager::getInstance()->parseParentBillingData(body);
        if(callback)
        {
            callback(true, 200);
        }
    };
    
    auto onFailed = [callback](const std::string& tag, long errorCode){
        if(callback)
        {
            callback(false, errorCode);
        }
    };
    
    HttpRequestCreator* request = API::UpdateBillingDataRequest(ParentManager::getInstance()->getLoggedInParentId(), onSuccess, onFailed);
    request->execute();
}

void UserAccountManager::getChildrenForLoggedInParent(const OnCompleteCallback& callback)
{
    auto onSuccess = [callback](const std::string& tag, const std::string& headers, const std::string& body){
        ParentManager::getInstance()->parseAvailableChildren(body);
        if(callback)
        {
            callback(true, 200);
        }
    };
    
    auto onFailed = [callback](const std::string& tag, long errorCode){
        if(callback)
        {
            callback(false, errorCode);
        }
    };
    
    HttpRequestCreator* request = API::GetAvailableChildrenRequest(onSuccess, onFailed);
    request->execute();
}

void UserAccountManager::loginChild(const std::string& profileName, const OnCompleteCallback& callback)
{
    auto onSuccess = [callback](const std::string& tag, const std::string& headers, const std::string& body){
        if((!ParentManager::getInstance()->parseChildLoginData(body)))
        {
            if(callback)
            {
                callback(false, 200);
            }
        }
        ParentManager::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey(API::kAZCountryCodeKey, headers));
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        IosNativeFunctionsSingleton::getInstance()->deleteHttpCookies(); //ios handles cookies on OS level. Removal of earlier cookies is important to avoid watching premium content with a free user.
        #endif
                
        const std::string& userId = ChildManager::getInstance()->getParentOrChildId();
        const std::string& sessionId = ChildManager::getInstance()->getParentOrChildCdnSessionId();
            
        auto onSuccess = [callback](const std::string& tag, const std::string& headers, const std::string& body){
            CookieManager::getInstance()->parseDownloadCookies(headers);
            ContentItemPoolDownloadHandler::getInstance()->getLatestData([callback](bool success){ //on complete
                if(success)
                {
                    HQStructureDownloadHandler::getInstance()->getLatestData([callback](bool success){ //on complete
                        if(success)
                        {
                            if(callback)
                            {
                                callback(true, 200);
                            }
                        }
                        else
                        {
                            if(callback)
                            {
                                callback(false, 200);
                            }
                        }
                    });
                }
                else
                {
                    if(callback)
                    {
                        callback(false, 200);
                    }
                }
            });
        };
            
        auto onFailed = [callback](const std::string& tag, long errorCode){
            if(callback)
            {
                callback(false, errorCode);
            }
        };
            
        HttpRequestCreator* request = API::GetSessionCookiesRequest(userId, sessionId, onSuccess, onFailed);
        request->execute();
    };
    
    auto onFailed = [callback](const std::string& tag, long errorCode){
        if(callback)
        {
            callback(false, errorCode);
        }
    };
    
    HttpRequestCreator* request = API::ChildLoginRequest(profileName, onSuccess, onFailed);
    request->execute();
}

void UserAccountManager::logout()
{
    ParentManager::getInstance()->logoutParent();
}

NS_AZOOMEE_END
