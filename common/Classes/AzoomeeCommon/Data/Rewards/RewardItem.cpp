//
//  RewardItem.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 11/02/2019.
//

#include "RewardItem.h"

NS_AZOOMEE_BEGIN

RewardItemRef RewardItem::createWithJson(const rapidjson::Value& rewardData)
{
	RewardItemRef reward = create();
	reward->parseRewardData(rewardData);
	return reward;
}
RewardItemRef RewardItem::create()
{
	return RewardItemRef(new RewardItem);
}

RewardItem::RewardItem()
{
	
}

void RewardItem::parseRewardData(const rapidjson::Value& rewardData)
{
	_id = getStringFromJson("id", rewardData);
	_userId = getStringFromJson("userId", rewardData);
	_itemPrice = getIntFromJson("itemPrice", rewardData);
	_description = getStringFromJson("descripion", rewardData);
	_status = getStringFromJson("status", rewardData);
	
	if(rewardData.HasMember("item"))
	{
		const rapidjson::Value& rewardItem = rewardData["item"];
	
		_itemId = getStringFromJson("itemId", rewardItem);
		_type = getStringFromJson("type", rewardItem);
		_uri = getStringFromJson("uri", rewardItem);
		_name = getStringFromJson("name", rewardItem);
	}
}

std::string RewardItem::getId() const
{
	return _id;
}
std::string RewardItem::getType() const
{
	return _type;
}
std::string RewardItem::getUserId() const
{
	return _userId;
}
std::string RewardItem::getItemId() const
{
	return _itemId;
}
std::string RewardItem::getDescription() const
{
	return _description;
}
int RewardItem::getItemPrice() const
{
	return _itemPrice;
}
std::string RewardItem::getStatus() const
{
	return _status;
}
std::string RewardItem::getUri() const
{
	return _uri;
}
std::string RewardItem::getName() const
{
	return _name;
}

NS_AZOOMEE_END
