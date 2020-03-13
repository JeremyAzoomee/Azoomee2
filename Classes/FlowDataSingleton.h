#ifndef __FLOW_DATA_SINGLETON_H__
#define __FLOW_DATA_SINGLETON_H__

#include "cocos2d.h"
#include <TinizineCommon/Tinizine.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

enum FlowTypeEnum { flowType_None, flowType_Signup, flowType_SignupNewProfile,flowType_NewProfile};
enum SuccessFailPath {SUCCESS_FAIL_NONE, IAP_SUCCESS, IAP_FAIL, SIGNUP_SUCCESS, SIGNUP_FAIL, ADDCHILD_SUCCESS, ADDCHILD_FAIL, PREMIUM_NEW_ACCOUNT, PREMIUM_EXISTING_ACCOUNT};

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
    
    FlowTypeEnum _currentFlowType;
    SuccessFailPath _currentSuccesFailPath;
    
    //Settings Deleted Child Flow
    bool childDeletedFlag = false;
    
    bool _displayUserPaidFlag = false;
    
public:
    static FlowDataSingleton* getInstance(void);
    virtual ~FlowDataSingleton();
    bool init(void);
    
    void setFlowToLogin();
    void setFlowToSignup(std::string userName, std::string password);
    void setFlowToNewProfile();
    void setFlowToSignUpNewProfile();
    
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
    
    //Settings Deleted Child Flow
    void setChildDeletedFlag();
    void resetChildDeletedFlag();
    bool getChildDeletedFlag();
    
    void setSuccessFailPath(SuccessFailPath path);
    SuccessFailPath getSuccessFailPath();
    
    void setDisplayUserPaidFlag(bool displayFlag);
    bool getDisplayUserPaidFlag();
    
};

NS_AZ_END

#endif
