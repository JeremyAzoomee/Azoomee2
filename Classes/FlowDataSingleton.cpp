#include "FlowDataSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "OrientationFunctions_ios.h"
#endif

USING_NS_CC;

#pragma mark - Constants

const char* const FlowDataSingleton::DataKeyUserName = "keyUserName";
const char* const FlowDataSingleton::DataKeyPassword = "keyPassword";
const char* const FlowDataSingleton::DataKeyErrorCode = "keyErrorCode";
const char* const FlowDataSingleton::DataKeyChildName = "keyChildName";
const char* const FlowDataSingleton::DataKeyOomeeColourNumber = "keyOomeeColourNumber";
const char* const FlowDataSingleton::DataKeyChildRefNumber = "keyChildRefNumber";

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

void FlowDataSingleton::setFlowToSignup(std::string userName, std::string password)
{
    clearData();
    flowData[DataKeyUserName] = userName;
    flowData[DataKeyPassword] = password;
}

void FlowDataSingleton::setFlowToAddProfile(std::string childName, int oomeeColourNumber)
{
    
}

void FlowDataSingleton::addChildData(std::string childName, int oomeeColourNumber)
{
    flowData[DataKeyChildName] = childName;
    flowData[DataKeyOomeeColourNumber] = StringUtils::format("%d",oomeeColourNumber);
}

void FlowDataSingleton::clearData()
{
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
    
    flowData[DataKeyErrorCode] = "";
    
    return rtrValue;
}

bool FlowDataSingleton::hasError()
{
    if(flowData.find( DataKeyErrorCode ) != flowData.end())
        if(flowData.at(DataKeyErrorCode) != "")
            return true;
    
    return false;
}

#pragma mark - Which Flow

bool FlowDataSingleton::isSignupFlow()
{
    return keyValueExists(DataKeyUserName);
}

bool FlowDataSingleton::isNewProfileFlow()
{
    return !keyValueExists(DataKeyChildRefNumber) && keyValueExists(DataKeyChildName);
}

bool FlowDataSingleton::isEditProfileFlow()
{
    return keyValueExists(DataKeyChildRefNumber);
}

#pragma mark - Private Functions

bool FlowDataSingleton::keyValueExists(std::string keyName)
{
    if(flowData.find( keyName ) != flowData.end())
        if(flowData.at(keyName) != "")
            return true;
    
    return false;
}

std::string FlowDataSingleton::getStringValue(std::string keyName)
{
    if(flowData.find( keyName ) != flowData.end())
        if(flowData.at(keyName) != "")
            return flowData[keyName];
    
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
