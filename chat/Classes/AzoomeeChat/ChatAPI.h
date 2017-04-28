#ifndef AzoomeeChat_ChatAPI_h
#define AzoomeeChat_ChatAPI_h

#include "AzoomeeChat.h"
#include <AzoomeeCommon/API/API.h>


NS_AZOOMEE_CHAT_BEGIN

class ChatAPI : public HttpRequestCreatorResponseDelegate
{
private:
    
    std::map<std::string, std::string> _contactList;
    
    // - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
public:
    
    static ChatAPI* getInstance();
    
    // Login the user.
    // TODO: Refactor this so the logic is common and not duplicated across here and BackEndCaller in azoomee2.
    void loginUser(const std::string& username, const std::string& password);
};

NS_AZOOMEE_CHAT_END

#endif
