//
//  RewardItem.h
//  TinizineCommon
//
//  Created by Macauley on 08/02/2019.
//

#ifndef RewardItem_h
#define RewardItem_h

#include <TinizineCommon/Tinizine.h>
#include <string>
#include <memory>
#include <TinizineCommon/Data/Json.h>
#include <TinizineCommon/Data/Child/InventoryItem.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class RewardItem;
typedef std::shared_ptr<RewardItem> RewardItemRef;

class RewardItem
{
protected:
	std::string _id;
	std::string _userId;
	std::string _description;
	int _itemPrice;
	std::string _status;
	
	InventoryItemRef _item = nullptr;
	
	RewardItem();
public:
	
	static RewardItemRef createWithJson(const rapidjson::Value& rewardData);
	static RewardItemRef createCoinReward(int itemPrice);
	
	void parseRewardData(const rapidjson::Value& rewardData);
	void mergeRewards(const RewardItemRef& reward);
	
	std::string getId() const;
	std::string getType() const;
	std::string getUserId() const;
	int getItemId() const;
	std::string getDescription() const;
	std::string getUri() const;
	int getItemPrice() const;
	std::string getStatus() const;
	std::string getName() const;
	
};


NS_AZ_END

#endif /* RewardItem_h */
