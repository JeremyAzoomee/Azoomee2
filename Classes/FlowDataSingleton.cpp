#include "FlowDataSingleton.h"

USING_NS_CC;

#pragma mark - Constants

const char* const DataKeyUserName = "keyUserName";
const char* const DataKeyPassword = "keyPassword";
const char* const DataKeyErrorCode = "keyErrorCode";
const char* const DataKeyChildName = "keyChildName";
const char* const DataKeyOomeeColourNumber = "keyOomeeColourNumber";
const char* const DataKeyChildRefNumber = "keyChildRefNumber";

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

void FlowDataSingleton::clearData()
{
    for(std::map<std::string, std::string>::iterator it = flowData.begin(); it != flowData.end(); it++)
    {
        flowData.erase(it);
    }
}

#pragma mark - Error Handling

void FlowDataSingleton::setErrorCode(long errorCode)
{
    flowData[DataKeyErrorCode] = StringUtils::format("%ld",errorCode);
}

long FlowDataSingleton::getErrorCode(long errorCode)
{
    if(hasError())
        return std::atoi(flowData.at(DataKeyErrorCode).c_str());
    else
        return 0;
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
    return !keyValueExists(DataKeyChildRefNumber);
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

