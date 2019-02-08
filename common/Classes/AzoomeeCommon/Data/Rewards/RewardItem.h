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

NS_AZOOMEE_BEGIN

class RewardItem;
typedef std::shared_ptr<RewardItem> RewardItemRef;

class RewardItem
{
private:
	std::string _id;
	std::string _type;
	std::string _name;
	std::string _description;
	
	int _value;
	
	RewardItem();
public:
	
	static RewardItemRef createWithJson(const rapidjson::Value& rewardData);
	static RewardItemRef create();
	
	void parseRewardData(const rapidjson::Document& rewardData);
	
	std::string getId() const;
	std::string getType() const;
	std::string getName() const;
	std::string getDescription() const;
	int getValue() const;
	
};


NS_AZOOMEE_END

#endif /* RewardItem_h */
