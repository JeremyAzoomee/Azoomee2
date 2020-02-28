//
//  FriendRequest.cpp
//  TinizineCommon
//
//  Created by Macauley on 18/01/2019.
//

#include "FriendRequest.h"

NS_TZ_BEGIN

FriendRequestRef FriendRequest::createWithJson(const rapidjson::Value& friendRequestData)
{
	FriendRequestRef friendRequest = FriendRequestRef(new FriendRequest);
	
	friendRequest->_senderName = getStringFromJson("senderName", friendRequestData);
	friendRequest->_friendName = getStringFromJson("friendName", friendRequestData);
	friendRequest->_inviteeCode = getStringFromJson("inviteeCode", friendRequestData);
	friendRequest->_id = getStringFromJson("id", friendRequestData);
	friendRequest->_senderId = getStringFromJson("senderId", friendRequestData);
	friendRequest->_respondentId = getStringFromJson("respondentId", friendRequestData);
	
	return friendRequest;
}
FriendRequestRef FriendRequest::create()
{
	return FriendRequestRef(new FriendRequest());
}

FriendRequest::FriendRequest()
{
	
}

std::string FriendRequest::getSenderName() const
{
	return _senderName;
}
std::string FriendRequest::getFriendName() const
{
	return _friendName;
}
std::string FriendRequest::getInviteeCode() const
{
	return _inviteeCode;
}
std::string FriendRequest::getId() const
{
	return _id;
}
std::string FriendRequest::getSenderId() const
{
	return _senderId;
}
std::string FriendRequest::getRespondentId() const
{
	return _respondentId;
}

NS_TZ_END
