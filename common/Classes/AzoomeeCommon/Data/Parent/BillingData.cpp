//
//  BillingData.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 03/12/2018.
//

#include "BillingData.h"

NS_AZOOMEE_BEGIN

BillingData::BillingData()
{
	
}

BillingDataRef BillingData::createWithJson(const rapidjson::Document& billingData)
{
	BillingDataRef data = BillingDataRef(new BillingData);
	
	if(billingData.HasParseError())
	{
		cocos2d::log("Billing Parse Error");
		return data;
	}
	
	
	const std::string& _billingStatusStr = getStringFromJson("billingStatus", billingData);
	if(_billingStatusStr == "SUBSCRIBED")
	{
		data->_billingStatus = BillingStatus::SUBSCRIBED;
	}
	const rapidjson::Value& purchases = billingData["purchases"];
	if(purchases.Size() > 1)
	{
		std::string nextBillingDate = getStringFromJson("nextBillDate", purchases[0]);
		for(int i = 1; i < purchases.Size(); i++)
		{
			if(nextBillingDate.compare(getStringFromJson("nextBillDate", purchases[i])) > 0)
			{
				nextBillingDate = getStringFromJson("nextBillDate", purchases[i]);
			}
		}
		
		data->_nextBillDate = nextBillingDate;
	}
	
	data->_paymentProvider = getStringFromJson("paymentProvider", billingData);
	
	return data;
}

std::string BillingData::getBillingStatusStr() const
{
	switch (_billingStatus) {
		case BillingStatus::SUBSCRIBED:
			return "SUBSCRIBED";
	}
}

BillingStatus BillingData::getBillingStatus() const
{
	return _billingStatus;
}
std::string BillingData::getStartDate() const
{
	return _startDate;
}
std::string BillingData::getEndDate() const
{
	return _endDate;
}
std::string BillingData::getNextBillDate() const
{
	return _nextBillDate;
}
std::string BillingData::getPaymentProvider() const
{
	return _paymentProvider;
}
std::string BillingData::getDuration() const
{
	return _duration;
}

NS_AZOOMEE_END
