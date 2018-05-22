//
//  ContentItemPoolHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef ContentItemPoolHandler_h
#define ContentItemPoolHandler_h

#include "../../Azoomee.h"
#include "../../API/HttpRequestCreator.h"

NS_AZOOMEE_BEGIN

class ContentItemPoolHandler : public HttpRequestCreatorResponseDelegate
{
private:
    
public:
    static ContentItemPoolHandler* getInstance();
    ~ContentItemPoolHandler();
    
    void getLatestContentPool();
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END

#endif /* ContentItemPoolHandler_h */
