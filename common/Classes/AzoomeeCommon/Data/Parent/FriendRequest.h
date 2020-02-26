//
//  FriendRequest.h
//  AzoomeeCommon
//
//  Created by Macauley on 18/01/2019.
//

#ifndef FriendRequest_h
#define FriendRequest_h

#include "../../Tinizine.h"
#include <string>
#include <memory>
#include "../Json.h"

NS_TZ_BEGIN

class FriendRequest;
typedef std::shared_ptr<FriendRequest> FriendRequestRef;

class FriendRequest
{
private:
	
	std::string _senderName;
	std::string _friendName;
	std::string _inviteeCode;
	std::string _id;
	std::string _senderId;
	std::string _respondentId;
	
	FriendRequest();
public:
	static FriendRequestRef createWithJson(const rapidjson::Value& friendRequestData);
	static FriendRequestRef create();
	
	std::string getSenderName() const;
	std::string getFriendName() const;
	std::string getInviteeCode() const;
	std::string getId() const;
	std::string getSenderId() const;
	std::string getRespondentId() const;
};

NS_TZ_END

#endif /* FriendRequest_h */
