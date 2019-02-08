//
//  RewardCallbackHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardCallbackHandler.h"

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
	// break down required params, create body string for the required params,
	// call sendRewardCallback(const std::string& callbackUrl, const std::string& callbackBody) with values
}
void RewardCallbackHandler::sendRewardCallback(const std::string& callbackUrl, const std::string& callbackBody)
{
	
}

void RewardCallbackHandler::setDelegate(RewardCallbackDelegate* delegate)
{
	_delegate = delegate;
}

//delegate functions
void RewardCallbackHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(_delegate)
	{
		//send data response to delegate
	}
}
void RewardCallbackHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	// do nothing
}

NS_AZOOMEE_END
