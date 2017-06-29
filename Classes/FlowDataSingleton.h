#ifndef __FLOW_DATA_SINGLETON_H__
#define __FLOW_DATA_SINGLETON_H__

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

enum FlowTypeEnum { flowType_None, flowType_Signup, flowType_SignupNewProfile,flowType_NewProfile};

class FlowDataSingleton : public cocos2d::Ref
{
private:
    std::string getStringValue(std::string keyName);
    
    std::map<std::string, std::string> flowData;
    
    //Data Keys
    static const char* const DataKeyUserName;
    static const char* const DataKeyPassword;
    static const char* const DataKeyErrorCode;
    static const char* const DataKeyChildName;
    static const char* const DataKeyOomeeColourNumber;
    static const char* const DataKeyChildRefNumber;
    static const char* const DataKeyIAPSuccess;
    
    FlowTypeEnum currentFlowType;
    
public:
    static FlowDataSingleton* getInstance(void);
    virtual ~FlowDataSingleton();
    bool init(void);
    
    void setFlowToLogin();
    void setFlowToSignup(std::string userName, std::string password);
    void setFlowToNewProfile();
    
    void addChildData(std::string childName, int oomeeColourNumber);
    void addIAPSuccess(bool IAPSuccess);

    bool isSignupFlow();
    bool isSignupNewProfileFlow();
    bool isNewProfileFlow();
    void setErrorCode(long errorCode);
    
    bool hasError();
    
    void clearData();
    
    std::string getUserName();
    std::string getPassword();
    long getErrorCode();
    std::string getChildName();
    int getOomeeColourNumber();
    int getChildRefNumber();
    bool getIAPSuccess();
};

NS_AZOOMEE_END

#endif
