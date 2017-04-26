#ifndef AzoomeeCommon_API_h
#define AzoomeeCommon_API_h

#include "../Azoomee.h"
#include <string>
#include "HttpRequestCreator.h"


NS_AZOOMEE_BEGIN

class API
{
public:
    static HttpRequestCreator* LoginRequest(const std::string& username,
                                            const std::string& password,
                                            HttpRequestCreatorResponseDelegate* delegate);
};
  
NS_AZOOMEE_END

#endif
