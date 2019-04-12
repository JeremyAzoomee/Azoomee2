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
#include "../Data/Json.h"
#include "../Data/Rewards/RewardItem.h"

NS_AZOOMEE_BEGIN

class RewardCallbackDelegate
{
public:
	virtual void onRewardSuccess(const RewardItemRef& reward) = 0;
};

class RewardCallbackHandler : public HttpRequestCreatorResponseDelegate
{
private:
	RewardCallbackHandler();
	
	RewardCallbackDelegate* _delegate = nullptr;
	
public:
	static RewardCallbackHandler* getInstance();
	virtual ~RewardCallbackHandler();
	
	void sendRewardCallback(const std::string& callbackData);
	
	void setDelegate(RewardCallbackDelegate* delegate);
	
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
	void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END


#endif /* RewardCallbackHandler_h */
