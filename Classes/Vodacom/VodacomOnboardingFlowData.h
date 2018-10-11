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
enum class ErrorType {NONE, LOGIN, VOUCHER, RESET};

class VodacomOnboardingFlowData;
typedef std::shared_ptr<VodacomOnboardingFlowData> VodacomOnboardingFlowDataRef;

class VodacomOnboardingFlowData
{
private:
	std::string _voucherCode;
	std::string _email;
	std::string _password;
	std::string _pin;
	
	UserType _userType = UserType::ANON;
	FlowState _currentState = FlowState::DETAILS;
	FlowState _prevState = FlowState::DETAILS;
	ErrorType _errorType = ErrorType::NONE;
	
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
	
	void setUserType(const UserType& userType);
	UserType getUserType() const;
	
	void setCurrentState(const FlowState& currentState);
	FlowState getCurrentState() const;
	
	void setPrevState(const FlowState& prevState);
	FlowState gePrevState() const;
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingFlowData_h */
