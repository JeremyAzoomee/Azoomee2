//
//  ContentItemPoolHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "ContentItemPoolHandler.h"
#include "ContentItemPoolParser.h"
#include "../../API/API.h"

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentItemPoolHandler> sContentItemPoolHandlerSharedInstance;

ContentItemPoolHandler* ContentItemPoolHandler::getInstance()
{
    if(!sContentItemPoolHandlerSharedInstance.get())
    {
        sContentItemPoolHandlerSharedInstance.reset(new ContentItemPoolHandler());
    }
    return sContentItemPoolHandlerSharedInstance.get();
}

ContentItemPoolHandler::~ContentItemPoolHandler(void)
{
    
}

void ContentItemPoolHandler::getLatestContentPool()
{
    
    bool needUpdate = false;
    //check if we need updated content pool from server
    if(needUpdate)
    {
        HttpRequestCreator* request = API::GetContentPoolRequest(this);
        request->execute();
    }
}

//delegate functions
void ContentItemPoolHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    ContentItemPoolParser::getInstance()->parseContentItemPool(body);
}

void ContentItemPoolHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    
}

NS_AZOOMEE_END
