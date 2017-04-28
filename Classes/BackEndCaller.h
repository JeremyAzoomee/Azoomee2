#ifndef Azoomee_BackEndCaller_h
#define Azoomee_BackEndCaller_h

#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>
#include <external/json/document.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>


// forward ref
class AwaitingAdultPinLayer;

class BackEndCaller : public cocos2d::Ref, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    bool accountJustRegistered;
    bool newChildJustRegistered;
    std::string newChildName;
    int oomeeAvatarNumber;
    //Saved here from registerParent, if onRegisterParentAnswerReceived success, then login.
    std::string registerParentUsername;
    std::string registerParentPassword;
    
    AwaitingAdultPinLayer* callBackNode;
    
private:
    
    // Login API success
    void onLoginAnswerReceived(const std::string& responseString);
    // Update billing API success
    void onUpdateBillingDataAnswerReceived(const std::string& responseString);
    // Update parent PIN API success
    void onUpdateParentPinAnswerReceived(const std::string& responseString);
    // Register parent API success
    void onRegisterParentAnswerReceived();
    // Get children API success
    void onGetChildrenAnswerReceived(const std::string& responseString);
    // Child login success
    void onChildLoginAnswerReceived(const std::string& responseString);
    // Gorden returned!
    void onGetGordonAnswerReceived(const std::string& responseString);
    // Register child API success
    void onRegisterChildAnswerReceived();
    
    void displayLoadingScreen();
    void hideLoadingScreen();
    void getBackToLoginScreen(long errorCode);
    
    //-HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    
public:
    
    // Returns the shared instance of the BackEndCaller
    static BackEndCaller* getInstance();
    virtual ~BackEndCaller();
    
    bool init();
    
    // Login a user
    void login(const std::string& username, const std::string& password);
    // Update billing information from the server
    void updateBillingData();
    // Update parent PIN from the server, calling back to AwaitingAdultPinLayer as part of the flow
    void updateParentPin(AwaitingAdultPinLayer* callBackTo);
    // Register a new parent account
    void registerParent(const std::string& emailAddress, const std::string& password, const std::string& pinNumber);
    // Get the children linked to the current parent from the backend
    void getAvailableChildren();
    // Login child profile with index
    void childLogin(int childNumber);
    // Register a new child profile on the current parent
    void registerChild(const std::string& childProfileName, const std::string& childGender, const std::string& childDOB, int oomeeNumber);
    // Get gorden. Good gorden.
    void getGordon();
    // Verify a google payment
    void verifyGooglePayment(const std::string& orderId, const std::string& iapSku, const std::string& purchaseToken);
    // Verify an Amazon payment
    void verifyAmazonPayment(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserid);
    // Verify an Apple payment
    void verifyApplePayment(const std::string& receiptData);
    // Get HQ content
    void getHQContent(const std::string& url, const std::string& category);
    
    
};

#endif
