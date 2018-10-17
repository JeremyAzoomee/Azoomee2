//
//  VodacomOnboardingFlowData.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingFlowData_h
#define VodacomOnboardingFlowData_h

#include <memory>
#include <string>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

enum class UserType {ANON, FREE, ANON_FREE}; //ANON = anonymous, FREE = logged in w/o premium, ANON_FREE = anonymous user who has account to log into
enum class FlowState {EXIT, DETAILS, ADD_VOUCHER, REGISTER, PIN, ADD_CHILD, LOGIN, SUCCESS, ERROR};
enum class ErrorType {NONE, LOGIN, VOUCHER, RESET, ALREADY_PREMIUM, ALREADY_REGISTERED};

class VodacomOnboardingFlowData;
typedef std::shared_ptr<VodacomOnboardingFlowData> VodacomOnboardingFlowDataRef;

class VodacomOnboardingFlowData
{
private:
	std::string _voucherCode;
	std::string _email;
	std::string _password;
	std::string _pin;
	std::string _childName;
	std::string _childAge;
	
	UserType _userType = UserType::ANON;
	ErrorType _errorType = ErrorType::NONE;
	
	std::stack<FlowState> _stateStack;
	
public:
	
	static VodacomOnboardingFlowDataRef create();
	VodacomOnboardingFlowData();
	~VodacomOnboardingFlowData();
	
	void setVoucherCode(const std::string& voucherCode);
	std::string getVoucherCode() const;
	
	void setEmail(const std::string& email);
	std::string getEmail() const;
	
	void setPassword(const std::string& password);
	std::string getPassword() const;
	
	void setPin(const std::string& pin);
	std::string getPin() const;
	
	void setChildName(const std::string& childName);
	std::string getChildName() const;
	
	void setChildAge(const std::string& childAge);
	std::string getChildAge() const;
	
	void setUserType(const UserType& userType);
	UserType getUserType() const;
	
	void setErrorType(const ErrorType& errorType);
	ErrorType getErrorType() const;
	
	void pushState(const FlowState& state);
	void popState();
	void resetStateStack();
	FlowState getCurrentState() const;
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingFlowData_h */
