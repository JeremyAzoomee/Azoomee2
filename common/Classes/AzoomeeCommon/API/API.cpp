#include "API.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

HttpRequestCreator* API::LoginRequest(const std::string& username,
                                      const std::string& password,
                                      HttpRequestCreatorResponseDelegate* delegate)
{
    HttpRequestCreator* request = new HttpRequestCreator(delegate);
    request->requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    request->requestTag = "parentLogin";
    request->method = "POST";
    return request;
}

NS_AZOOMEE_END
