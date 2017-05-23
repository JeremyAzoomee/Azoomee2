#ifndef AzoomeeChat_AuthAPI_h
#define AzoomeeChat_AuthAPI_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/API.h>
#include <map>


NS_AZOOMEE_BEGIN

// forward decleration
class AuthAPIObserver;

/**
 * Manages communication of auth APIs with the Azoomee server.
 * TODO: Consider moving this to AzoomeeCommon.
 */
class AuthAPI : private HttpRequestCreatorResponseDelegate
{
private:
    
    /// Observers monitoring this API and it's responses
    std::vector<AuthAPIObserver*> _observers;
    
    // Private construction - Use ::getInstance()
    AuthAPI();
    
    // - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
public:
    
    static AuthAPI* getInstance();
    
    /// Register an observer
    void registerObserver(AuthAPIObserver* observer);
    /// Remove a previously registered observer
    void removeObserver(AuthAPIObserver* observer);
    
    
    /// Returns true if we're currently logged in
    bool isLoggedIn() const;
    /// Login the user
    /// Response: AuthAPIObserver::onAuthAPILogin
    void loginUser(const std::string& username, const std::string& password);
    /// Logout the user
    void logoutUser();
    
    /// Requests the latest child accounts
    /// Response: AuthAPIObserver::onAuthAPIGetAvailableChildren
    void getAvailableChildren();
    
    /// Returns true if a child is logged in
    bool isChildLoggedIn() const;
    /// Login the user
    /// Response: AuthAPIObserver::onAuthAPIChildLogin
    void loginChild(const std::string& profileName);
    /// Logout the child
    void logoutChild();
};

/**
 * Recieve events relating to the chat API.
 */
struct AuthAPIObserver
{
    virtual void onAuthAPILogin() {};
    virtual void onAuthAPIGetAvailableChildren() {};
    virtual void onAuthAPIChildLogin() {};
    virtual void onAuthAPIRequestFailed(const std::string& requestTag, long errorCode) {};
};

NS_AZOOMEE_END

#endif
