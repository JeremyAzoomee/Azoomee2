#ifndef Azoomee_BackEndCaller_h
#define Azoomee_BackEndCaller_h

#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>
#include <external/json/document.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>


class BackEndCaller : public cocos2d::Ref, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    bool accountJustRegistered;
    bool newChildJustRegistered;
    std::string newChildName;
    int oomeeAvatarNumber;
    
public:
    /** Returns the shared instance of the Game Manager */
    static BackEndCaller* getInstance(void);
    
    virtual ~BackEndCaller();
    bool init(void);
    
    void login(std::string username, std::string password);
    void onLoginAnswerReceived(std::string responseString);
    
    void updateBillingData();
    void onUpdateBillingDataAnswerReceived(std::string responseString);
    
    void updateParentPin(cocos2d::Node *callBackTo);
    void onUpdateParentPinAnswerReceived(std::string responseString);
    
    void registerParent(std::string emailAddress, std::string password, std::string pinNumber);
    void onRegisterParentAnswerReceived();
    
    void getAvailableChildren();
    void onGetChildrenAnswerReceived(std::string responseString);
    
    void childLogin(int childNumber);
    void onChildLoginAnswerReceived(std::string responseString);
    
    void registerChild(std::string childProfileName, std::string childGender, std::string childDOB, int oomeeNumber);
    void onRegisterChildAnswerReceived();
    
    void getHQContent(const std::string& url, const std::string& category);
    
    void getGordon();
    void onGetGordonAnswerReceived(std::string responseString);
    
    void verifyGooglePayment(const std::string& orderId, const std::string& iapSku, const std::string& purchaseToken);
    void verifyAmazonPayment(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserid);
    void verifyApplePayment(const std::string& receiptData);
    
    void displayLoadingScreen();
    void hideLoadingScreen();
    void getBackToLoginScreen(long errorCode);
    
    //-HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    //Saved here from registerParent, if onRegisterParentAnswerReceived success, then login.
    std::string registerParentUsername;
    std::string registerParentPassword;
    
    bool newSubscriptionJustStarted;

    
    //------------------------------------------------------------------------------------------------------------------
    
    cocos2d::Node *callBackNode;
    
};

#endif
