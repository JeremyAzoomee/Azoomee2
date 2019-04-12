//
//  RewardCallbackHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardCallbackHandler.h"
#include "../Data/Child/ChildDataProvider.h"
#include "../Utils/StringFunctions.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<RewardCallbackHandler> sRewardCallbackHandlerSharedInstance;

RewardCallbackHandler* RewardCallbackHandler::getInstance()
{
	if(!sRewardCallbackHandlerSharedInstance.get())
	{
		sRewardCallbackHandlerSharedInstance.reset(new RewardCallbackHandler());
	}
	return sRewardCallbackHandlerSharedInstance.get();
}
RewardCallbackHandler::~RewardCallbackHandler()
{
}
RewardCallbackHandler::RewardCallbackHandler()
{
}

void RewardCallbackHandler::sendRewardCallback(const std::string& callbackData)
{
	const std::vector<std::string>& urls = splitStringToVector(callbackData, ";");
	
	for(const std::string& url : urls)
	{
		HttpRequestCreator* request = API::RewardCallback(url, this);
		request->execute();
	}
}

void RewardCallbackHandler::setDelegate(RewardCallbackDelegate* delegate)
{
	_delegate = delegate;
}

//delegate functions
void RewardCallbackHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	rapidjson::Document data;
	data.Parse(body.c_str());
	if(data.HasParseError())
	{
		return;
	}
	RewardItemRef reward = RewardItem::createWithJson(data);
	if(reward->getStatus() == "PENDING")
	{
		if(_delegate)
		{
			//send data response to delegate
			_delegate->onRewardSuccess(reward);
		}
	}
}
void RewardCallbackHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	// do nothing
}

NS_AZOOMEE_END
