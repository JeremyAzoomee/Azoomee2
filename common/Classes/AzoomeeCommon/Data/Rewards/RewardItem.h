//
//  RewardItem.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/02/2019.
//

#ifndef RewardItem_h
#define RewardItem_h

#include "../../Azoomee.h"
#include <string>
#include <memory>
#include "../Json.h"
#include "InventoryItem.h"

NS_AZOOMEE_BEGIN

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
	static RewardItemRef create();
	
	void parseRewardData(const rapidjson::Value& rewardData);
	void mergeRewards(const RewardItemRef& reward);
	
	std::string getId() const;
	std::string getType() const;
	std::string getUserId() const;
	std::string getItemId() const;
	std::string getDescription() const;
	std::string getUri() const;
	int getItemPrice() const;
	std::string getStatus() const;
	std::string getName() const;
	
};


NS_AZOOMEE_END

#endif /* RewardItem_h */
