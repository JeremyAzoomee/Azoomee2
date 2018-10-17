//
//  VodacomOnboardingFlowData.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingFlowData.h"

NS_AZOOMEE_BEGIN

VodacomOnboardingFlowDataRef VodacomOnboardingFlowData::create()
{
	return std::make_shared<VodacomOnboardingFlowData>();
}

VodacomOnboardingFlowData::VodacomOnboardingFlowData()
{
	
}
VodacomOnboardingFlowData::~VodacomOnboardingFlowData()
{
	
}

void VodacomOnboardingFlowData::setVoucherCode(const std::string& voucherCode)
{
	_voucherCode = voucherCode;
}
std::string VodacomOnboardingFlowData::getVoucherCode() const
{
	return _voucherCode;
}

void VodacomOnboardingFlowData::setEmail(const std::string& email)
{
	_email = email;
}
std::string VodacomOnboardingFlowData::getEmail() const
{
	return _email;
}

void VodacomOnboardingFlowData::setPassword(const std::string& password)
{
	_password = password;
}
std::string VodacomOnboardingFlowData::getPassword() const
{
	return _password;
}

void VodacomOnboardingFlowData::setPin(const std::string& pin)
{
	_pin = pin;
}
std::string VodacomOnboardingFlowData::getPin() const
{
	return _pin;
}

void VodacomOnboardingFlowData::setChildName(const std::string &childName)
{
	_childName = childName;
}
std::string VodacomOnboardingFlowData::getChildName() const
{
	return _childName;
}

void VodacomOnboardingFlowData::setChildAge(const std::string& childAge)
{
	_childAge = childAge;
}
std::string VodacomOnboardingFlowData::getChildAge() const
{
	return _childAge;
}

void VodacomOnboardingFlowData::setUserType(const UserType& userType)
{
	_userType = userType;
}
UserType VodacomOnboardingFlowData::getUserType() const
{
	return _userType;
}

void VodacomOnboardingFlowData::setErrorType(const ErrorType& errorType)
{
	_errorType = errorType;
}
ErrorType VodacomOnboardingFlowData::getErrorType() const
{
	return _errorType;
}

void VodacomOnboardingFlowData::pushState(const FlowState &state)
{
	_stateStack.push(state);
}

void VodacomOnboardingFlowData::popState()
{
	if(_stateStack.size() > 0)
	{
		_stateStack.pop();
	}
	if(_stateStack.size() == 0)
	{
		_stateStack.push(FlowState::EXIT);
	}
}

void VodacomOnboardingFlowData::resetStateStack()
{
	while(_stateStack.size() > 1)
	{
		_stateStack.pop();
	}
}

FlowState VodacomOnboardingFlowData::getCurrentState() const
{
	return _stateStack.top();
}

NS_AZOOMEE_END
