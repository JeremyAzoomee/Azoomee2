//
//  AnonChildHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 24/01/2019.
//

#ifndef AnonChildHandler_h
#define AnonChildHandler_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>
#include "../Json.h"
#include "Child.h"
#include "../../API/API.h"

NS_AZOOMEE_BEGIN

class AnonChildHandler: public HttpRequestCreatorResponseDelegate
{
	typedef std::function<void(bool)> RegisterAnonChildCallback;
	
private:
	RegisterAnonChildCallback _registerAnonChildCallback = nullptr;
	std::string _newChildId;
	std::string _oldAnonId;
	
	void copyAnonUserData();
	
public:
	static AnonChildHandler* getInstance(void);
	
	virtual ~AnonChildHandler();
	bool init(void);
	
	void loginAnonChild(const std::string& loginDataStr);
	
	bool localAnonChildExists() const;
	ChildRef getLocalAnonChild() const;
	
	void registerAnonChildToUser(const RegisterAnonChildCallback& callback);
	
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END

#endif /* AnonChildHandler_h */
