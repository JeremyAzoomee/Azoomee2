//
//  RewardCallbackHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/02/2019.
//

#ifndef RewardCallbackHandler_h
#define RewardCallbackHandler_h

#include "../Azoomee.h"
#include "API.h"
#include <memory>

NS_AZOOMEE_BEGIN

class RewardCallbackDelegate
{
	virtual void onRewardSuccess(const std::string& responseData) = 0;
};

class RewardCallbackHandler : public HttpRequestCreatorDelegate
{
private:
	RewardCallbackHandler();
	
	RewardCallbackDelegate* _delegate = nullptr;
	
	std::string generateResponseBody(const std::vector<std::string>& paramIds);
	
public:
	static RewardCallbackHandler* getInstance();
	virtual ~RewardCallbackHandler();
	
	void sendRewardCallback(const std::string& callbackData);
	void sendRewardCallback(const std::string& callbackUrl, const std::string& callbackBody);
	
	void setDelegate(RewardCallbackDelegate* delegate);
	
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
	void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END


#endif /* RewardCallbackHandler_h */
