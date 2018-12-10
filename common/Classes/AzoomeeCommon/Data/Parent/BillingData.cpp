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
	else if(_billingStatusStr == "FREE_TRIAL")
	{
		data->_billingStatus = BillingStatus::FREE_TRIAL;
	}
	else if(_billingStatusStr == "FREE_REGISTERED")
	{
		data->_billingStatus = BillingStatus::FREE_REGISTERED;
	}
	else
	{
		data->_billingStatus = BillingStatus::ANON;
	}
	const rapidjson::Value& purchases = billingData["purchases"];
	if(purchases.Size() > 1)
	{
		std::string nextBillingDate = getStringFromJson("nextBillDate", purchases[1]);
		std::string paymentProvider = getStringFromJson("paymentProvider", purchases[1]);
		
		for(int i = 1; i < purchases.Size(); i++)
		{
			if(nextBillingDate.compare(getStringFromJson("nextBillDate", purchases[i])) > 0)
			{
				nextBillingDate = getStringFromJson("nextBillDate", purchases[i]);
				paymentProvider = getStringFromJson("paymentProvider", purchases[i]);
			}
		}
		
		data->_nextBillDate = nextBillingDate;
		data->_paymentProvider = paymentProvider;
	}
	if(billingData.HasMember("vouchers"))
	{
		const rapidjson::Value& vouchers = billingData["vouchers"];
		if(vouchers.IsArray() && vouchers.Size() > 0)
		{
			int endDate = getIntFromJson("endDate", vouchers[0]);
			std::string code = getStringFromJson("voucherCode", vouchers[0]);
			std::string campaign = getStringFromJson("campaign", vouchers[0]);
			std::string organization = getStringFromJson("organization", vouchers[0]);
			for(int i = 0; i < vouchers.Size(); i++)
			{
				int newDate = getIntFromJson("endDate", vouchers[i]);
				if(endDate < newDate)
				{
					endDate = newDate;
					code = getStringFromJson("voucherCode", vouchers[i]);
					campaign = getStringFromJson("campaign", vouchers[i]);
					organization = getStringFromJson("organization", vouchers[i]);
				}
			}
			data->_voucherCode = code;
			data->_campaign = campaign;
			data->_organisation = organization;
		}
	}
	
	data->_duration = getIntFromJson("totalSubscriptionDurationHours", billingData, 0);
	//data->_paymentProvider = getStringFromJson("paymentProvider", billingData);
	
	return data;
}

std::string BillingData::getBillingStatusStr() const
{
	switch (_billingStatus) {
		case BillingStatus::ANON:
			return "ANON";
		case BillingStatus::FREE_REGISTERED:
			return "FREE_REGISTERED";
		case BillingStatus::SUBSCRIBED:
			return "SUBSCRIBED";
		case BillingStatus::FREE_TRIAL:
			return "FREE_TRIAL";
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
int BillingData::getDuration() const
{
	return _duration;
}

std::string BillingData::getVoucherCode() const
{
	return _voucherCode;
}
std::string BillingData::getCampaign() const
{
	return _campaign;
}
std::string BillingData::getOrganisation() const
{
	return _organisation;
}

NS_AZOOMEE_END
