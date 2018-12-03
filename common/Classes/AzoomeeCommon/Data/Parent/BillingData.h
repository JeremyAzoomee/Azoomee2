//
//  BillingData.h
//  AzoomeeCommon
//
//  Created by Macauley on 03/12/2018.
//

#ifndef BillingData_h
#define BillingData_h

#include "../../Azoomee.h"
#include <string>
#include <memory>
#include "../Json.h"

NS_AZOOMEE_BEGIN

class BillingData;
typedef std::shared_ptr<BillingData> BillingDataRef;

enum class BillingStatus {SUBSCRIBED};

class BillingData
{
private:
	BillingStatus _billingStatus;
	std::string _startDate;
	std::string _endDate;
	std::string _nextBillDate;
	std::string _paymentProvider;
	std::string _duration;
	
	BillingData();
public:
	
	static BillingDataRef createWithJson(const rapidjson::Document& billingData);
	
	std::string getBillingStatusStr() const;
	BillingStatus getBillingStatus() const;
	std::string getStartDate() const;
	std::string getEndDate() const;
	std::string getNextBillDate() const;
	std::string getPaymentProvider() const;
	std::string getDuration() const;
	
};

NS_AZOOMEE_END

#endif /* BillingData_h */
