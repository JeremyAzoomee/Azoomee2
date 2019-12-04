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
	RewardItemRef reward = RewardItemRef(new RewardItem());
	reward->parseRewardData(rewardData);
	return reward;
}

RewardItemRef RewardItem::createCoinReward(int itemPrice)
{
	RewardItemRef reward = RewardItemRef(new RewardItem());
    reward->_itemPrice = itemPrice;
    reward->_status = "LOCAL";
    reward->_item = InventoryItem::createCoin();
    return reward;
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
	
		_item = InventoryItem::createWithJson(rewardItem);
	}
}

void RewardItem::mergeRewards(const RewardItemRef& reward)
{
	if(reward->getType() == "COIN" && (_id == "" || _item->getType() == "COIN")) // only merge is this is uninitialised or both rewards are coins
	{
		if(_id == "")// this is unitintialised, copy merged reward
		{
			_id = reward->_id;
			_item = reward->_item;
			_description = reward->_description;
			_status = reward->_status;
			_userId = reward->_userId;
			_itemPrice = reward->_itemPrice;
		}
		else // append new id and reward value
		{
			_id += ";" + reward->_id;
			_itemPrice += reward->_itemPrice;
		}
	}
}

std::string RewardItem::getId() const
{
	return _id;
}
std::string RewardItem::getType() const
{
	return _item->getType();
}
std::string RewardItem::getUserId() const
{
	return _userId;
}
int RewardItem::getItemId() const
{
	return _item->getItemId();
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
	return _item->getUri();
}
std::string RewardItem::getName() const
{
	return _item->getName();
}

NS_AZOOMEE_END
