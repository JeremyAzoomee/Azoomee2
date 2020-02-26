//
//  BillingData.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 03/12/2018.
//

#include "BillingData.h"

NS_TZ_BEGIN

const std::string BillingData::kBillingProviderApple = "APPLE";
const std::string BillingData::kBillingProviderGoogle = "GOOGLE";
const std::string BillingData::kBillingProviderAmazon = "AMAZON";
const std::string BillingData::kBillingProviderVodacomSA = "VODACOM_SA";

const std::map<BillingStatus, std::string> BillingData::kBillingStatusToStringMap = {
	{BillingStatus::ANON, "ANON"},
	{BillingStatus::FREE_REGISTERED, "FREE_REGISTERED"},
	{BillingStatus::FREE_TRIAL, "FREE_TRIAL"},
	{BillingStatus::SUBSCRIBED, "SUBSCRIBED"}
};

BillingData::BillingData()
{
	
}

BillingDataRef BillingData::createWithJson(const rapidjson::Document& billingData)
{
	BillingDataRef data = BillingDataRef(new BillingData);
	
	if(billingData.HasParseError())
	{
		cocos2d::log("Billing Parse Error");
		data->_billingStatus = BillingStatus::ANON;
		return data;
	}
	
	
	const std::string& billingStatusStr = getStringFromJson("billingStatus", billingData);
	for(const auto& pair : kBillingStatusToStringMap)
	{
		if(billingStatusStr == pair.second)
		{
			data->_billingStatus = pair.first;
			break;
		}
		else
		{
			data->_billingStatus = BillingStatus::ANON;
		}
	}
	
	if(billingData.HasMember("purchases"))
	{
		const rapidjson::Value& purchases = billingData["purchases"];
		if(purchases.IsArray() && purchases.Size() > 1)
		{
			// purchases[0] will always be the default FREE purchase object with free content entitlements, so start at element purchases[1]
			std::string nextBillingDate = getStringFromJson("nextBillDate", purchases[1]);
			std::string paymentProvider = getStringFromJson("paymentProvider", purchases[1]);
			
			for(int i = 1; i < purchases.Size(); i++)
			{
				const std::string& billDate = getStringFromJson("nextBillDate", purchases[i]);
				if(nextBillingDate.compare(billDate) > 0)
				{
					nextBillingDate = billDate;
					paymentProvider = getStringFromJson("paymentProvider", purchases[i]);
				}
			}
			
			data->_nextBillDate = nextBillingDate;
			data->_paymentProvider = paymentProvider;
		}
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
	
	return data;
}

std::string BillingData::getBillingStatusStr() const
{
	return kBillingStatusToStringMap.at(_billingStatus);
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

NS_TZ_END
