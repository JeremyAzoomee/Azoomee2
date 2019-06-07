#include "AuthAPI.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include <cocos/cocos2d.h>
#include <memory>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

#pragma mark - Init

static std::auto_ptr<AuthAPI> sAuthAPISharedInstance;

AuthAPI* AuthAPI::getInstance()
{
    if(!sAuthAPISharedInstance.get())
    {
        sAuthAPISharedInstance.reset(new AuthAPI());
    }
    return sAuthAPISharedInstance.get();
}

AuthAPI::AuthAPI()
{
    // Initialise logged in information
    ParentManager* parentManager = ParentManager::getInstance();
    if(parentManager->hasParentLoginDataInUserDefaults())
    {
        parentManager->retrieveParentLoginDataFromUserDefaults();
        return;
    }
}

#pragma mark - Observers

void AuthAPI::registerObserver(AuthAPIObserver* observer)
{
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if(it == _observers.end())
    {
        _observers.push_back(observer);
    }
}

void AuthAPI::removeObserver(AuthAPIObserver* observer)
{
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if(it != _observers.end())
    {
        _observers.erase(it);
    }
}

#pragma mark - User

bool AuthAPI::isLoggedIn() const
{
    ParentManager* parentData = ParentManager::getInstance();
    const std::string& parentId = parentData->getLoggedInParentId();
    return !parentId.empty();
}

void AuthAPI::loginUser(const std::string& username, const std::string& password)
{
    HttpRequestCreator* request = API::LoginRequest(username, password, this);
    request->execute();
}

void AuthAPI::logoutUser()
{
    logoutChild();
    ParentManager::getInstance()->clearParentLoginDataFromUserDefaults();
}

#pragma mark - Child

void AuthAPI::getAvailableChildren()
{
    HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
    request->execute();
}

bool AuthAPI::isChildLoggedIn() const
{
    ChildManager* childData = ChildManager::getInstance();
    const std::string& childId = childData->getParentOrChildId();
    return !childId.empty();
}

void AuthAPI::loginChild(const std::string& profileName)
{
    HttpRequestCreator* request = API::ChildLoginRequest(profileName, this);
    request->execute();
}

void AuthAPI::logoutChild()
{
    ChildManager::getInstance()->setChildLoggedIn(false);
}

#pragma mark - HttpRequestCreatorResponseDelegate

void AuthAPI::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
//    cocos2d::log("AuthAPI::onHttpRequestSuccess: %s, body=%s", requestTag.c_str(), body.c_str());
    ParentManager* parentData = ParentManager::getInstance();
    ChildManager* childData = ChildManager::getInstance();
    
    // Parent login success
    if(requestTag == API::TagLogin)
    {
        if(parentData->parseParentLoginData(body))
        {
            cocos2d::log("Logged in!");
            
            // Notify observers
            for(auto observer : _observers)
            {
                observer->onAuthAPILogin();
            }
        }
        else
        {
            onHttpRequestFailed(requestTag, ERROR_CODE_INVALID_CREDENTIALS);
        }
    }
    // Get children success
    else if(requestTag == API::TagGetAvailableChildren)
    {
        parentData->parseAvailableChildren(body);
        
        cocos2d::log("Child profiles:");
        for(int i = 0; i < parentData->getAmountOfAvailableChildren(); ++i)
        {
            const std::string& profileName = parentData->getProfileNameForAnAvailableChild(i);
            cocos2d::log("profileName=%s", profileName.c_str());
        }
        
        // Notify observers
        for(auto observer : _observers)
        {
            observer->onAuthAPIGetAvailableChildren();
        }
    }
    // Child login success
    else if(requestTag == API::TagChildLogin)
    {
        parentData->parseChildLoginData(body);
        cocos2d::log("Logged in as child: %s", childData->getLoggedInChildName().c_str());
        
        // Notify observers
        for(auto observer : _observers)
        {
            observer->onAuthAPIChildLogin();
        }
    }
}

void AuthAPI::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    cocos2d::log("AuthAPI::onHttpRequestFailed: %s, errorCode=%ld", requestTag.c_str(), errorCode);
    
    // Notify observers
    for(auto observer : _observers)
    {
        observer->onAuthAPIRequestFailed(requestTag, errorCode);
    }
}

NS_AZOOMEE_END
