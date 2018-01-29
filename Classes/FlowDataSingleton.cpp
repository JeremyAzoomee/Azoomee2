#include "FlowDataSingleton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

#pragma mark - Constants

const char* const FlowDataSingleton::DataKeyUserName = "keyUserName";
const char* const FlowDataSingleton::DataKeyPassword = "keyPassword";
const char* const FlowDataSingleton::DataKeyErrorCode = "keyErrorCode";
const char* const FlowDataSingleton::DataKeyChildName = "keyChildName";
const char* const FlowDataSingleton::DataKeyOomeeColourNumber = "keyOomeeColourNumber";
const char* const FlowDataSingleton::DataKeyChildRefNumber = "keyChildRefNumber";
const char* const FlowDataSingleton::DataKeyIAPSuccess = "keyIAPSuccess";

#pragma mark - SingletonSetup

static FlowDataSingleton *_sharedFlowDataSingletonSingleton = NULL;

FlowDataSingleton* FlowDataSingleton::getInstance()
{
    if (! _sharedFlowDataSingletonSingleton)
    {
        _sharedFlowDataSingletonSingleton = new FlowDataSingleton();
        _sharedFlowDataSingletonSingleton->init();
    }
    
    return _sharedFlowDataSingletonSingleton;
}

FlowDataSingleton::~FlowDataSingleton(void)
{
    
}

bool FlowDataSingleton::init(void)
{
    return true;
}

#pragma mark - Flow Setup

void FlowDataSingleton::setFlowToLogin()
{
    clearData();
}

void FlowDataSingleton::setFlowToSignup(std::string userName, std::string password)
{
    clearData();
    flowData[DataKeyUserName] = userName;
    flowData[DataKeyPassword] = password;
    _currentFlowType = flowType_Signup;
}

void FlowDataSingleton::setFlowToNewProfile()
{
    clearData();
    _currentFlowType = flowType_NewProfile;
}

void FlowDataSingleton::setFlowToSignUpNewProfile()
{
    _currentFlowType = flowType_SignupNewProfile;
}

void FlowDataSingleton::addChildData(std::string childName, int oomeeColourNumber)
{
    if(isSignupFlow())
    {
        _currentFlowType = flowType_SignupNewProfile;
    }
    
    flowData[DataKeyChildName] = childName;
    flowData[DataKeyOomeeColourNumber] = StringUtils::format("%d",oomeeColourNumber);
}

void FlowDataSingleton::addIAPSuccess(bool IAPSuccess)
{
    flowData[DataKeyIAPSuccess] = StringUtils::format("%d", IAPSuccess);
}

void FlowDataSingleton::clearData()
{
    _currentFlowType = flowType_None;
    flowData.clear();
}

#pragma mark - Error Handling

void FlowDataSingleton::setErrorCode(long errorCode)
{
    flowData[DataKeyErrorCode] = StringUtils::format("%ld",errorCode);
}

long FlowDataSingleton::getErrorCode()
{
    long rtrValue = 0;
    
    if(hasError())
        rtrValue = std::atoi(flowData.at(DataKeyErrorCode).c_str());
    
    flowData.erase(DataKeyErrorCode);
    
    return rtrValue;
}

bool FlowDataSingleton::hasError()
{
    if(flowData.find(DataKeyErrorCode) != flowData.end())
        return true;
    
    return false;
}

#pragma mark - Which Flow

bool FlowDataSingleton::isSignupFlow()
{
    return _currentFlowType == flowType_Signup;
}

bool FlowDataSingleton::isSignupNewProfileFlow()
{
    return _currentFlowType == flowType_SignupNewProfile;
}

bool FlowDataSingleton::isNewProfileFlow()
{
    return _currentFlowType == flowType_NewProfile;;
}

#pragma mark - Private Functions

std::string FlowDataSingleton::getStringValue(std::string keyName)
{
    if(flowData.find( keyName ) != flowData.end())
    {
        if(flowData.at(keyName) != "")
        {
            return flowData[keyName];
        }
    }
    
    return "";
}

#pragma mark - Get Functions

std::string FlowDataSingleton::getUserName()
{
    return getStringValue(DataKeyUserName);
}

std::string FlowDataSingleton::getPassword()
{
    return getStringValue(DataKeyPassword);
}

std::string FlowDataSingleton::getChildName()
{
    return getStringValue(DataKeyChildName);
}

int FlowDataSingleton::getOomeeColourNumber()
{
    return std::atoi(getStringValue(DataKeyOomeeColourNumber).c_str());
}

int FlowDataSingleton::getChildRefNumber()
{
    return std::atoi(getStringValue(DataKeyChildRefNumber).c_str());
}

bool FlowDataSingleton::getIAPSuccess()
{
    return getStringValue(DataKeyIAPSuccess) == "1";
}

//Settings Deleted Child Flow
void FlowDataSingleton::setChildDeletedFlag()
{
    childDeletedFlag = true;
}

void FlowDataSingleton::resetChildDeletedFlag()
{
    childDeletedFlag = false;
}

bool FlowDataSingleton::getChildDeletedFlag()
{
    return childDeletedFlag;
}

void FlowDataSingleton::setSuccessFailPath(SuccessFailPath path)
{
    _currentSuccesFailPath = path;
}

SuccessFailPath FlowDataSingleton::getSuccessFailPath()
{
    return _currentSuccesFailPath;
}

NS_AZOOMEE_END
