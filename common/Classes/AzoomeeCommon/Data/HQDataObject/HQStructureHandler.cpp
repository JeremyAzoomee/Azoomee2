//
//  HQStructureHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "HQStructureHandler.h"
#include "HQStructureParser.h"
#include <cocos/cocos2d.h>
#include "../../API/API.h"
#include "../../Utils/FileZipUtil.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<HQStructureHandler> sHQStructureHandlerSharedInstance;

HQStructureHandler* HQStructureHandler::getInstance()
{
    if(!sHQStructureHandlerSharedInstance.get())
    {
        sHQStructureHandlerSharedInstance.reset(new HQStructureHandler());
    }
    return sHQStructureHandlerSharedInstance.get();
}

HQStructureHandler::~HQStructureHandler(void)
{
    
}

void HQStructureHandler::getLatestHQStructureFeed()
{
    bool needUpdate = false;
    // check if need newer version
    if(needUpdate)
    {
        HttpRequestCreator* request = API::GetHQStructureDataRequest(this);
        request->execute();
    }
}

void HQStructureHandler::loadHQStructureForUser()
{
    // http call to get feed name for child?
}

void HQStructureHandler::loadHQStructureDataByName(const std::string& userFeedName)
{
    HQStructureParser::getInstance()->parseHQStructureData(FileUtils::getInstance()->getStringFromFile(userFeedName));
}

//delegate functions
void HQStructureHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    FileUtils::getInstance()->writeStringToFile(body, FileUtils::getInstance()->getWritablePath() + "hdDataCache/hqData.zip");
}

void HQStructureHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    
}

NS_AZOOMEE_END
