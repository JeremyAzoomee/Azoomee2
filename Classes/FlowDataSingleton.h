#ifndef __FLOW_DATA_SINGLETON_H__
#define __FLOW_DATA_SINGLETON_H__

#include "cocos2d.h"

class FlowDataSingleton : public cocos2d::Ref
{
private:
    bool keyValueExists(std::string keyName);
    std::string getStringValue(std::string keyName);
    
    std::map<std::string, std::string> flowData;
    
    //Data Keys
    static const char* const DataKeyUserName;
    static const char* const DataKeyPassword;
    static const char* const DataKeyErrorCode;
    static const char* const DataKeyChildName;
    static const char* const DataKeyOomeeColourNumber;
    static const char* const DataKeyChildRefNumber;
    
public:
    static FlowDataSingleton* getInstance(void);
    virtual ~FlowDataSingleton();
    bool init(void);
    
    void setFlowToSignup(std::string userName, std::string password);

    bool isSignupFlow();
    bool isNewProfileFlow();
    bool isEditProfileFlow();
    void setErrorCode(long errorCode);
    
    bool hasError();
    
    void clearData();
    
    std::string getUserName();
    std::string getPassword();
    long getErrorCode(long errorCode);
    std::string getChildName();
    int getOomeeColourNumber();
    int getChildRefNumber();
};

#endif
