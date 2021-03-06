#ifndef Azoomee_BackEndCaller_h
#define Azoomee_BackEndCaller_h

#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>
#include <external/json/document.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemPoolDownloadHandler.h>
#include <AzoomeeCommon/Data/HQDataObject/HQStructureDownloadHandler.h>

NS_AZOOMEE_BEGIN

// forward ref
class AwaitingAdultPinLayer;

class BackEndCaller : public cocos2d::Ref, public Azoomee::HttpRequestCreatorResponseDelegate
{
private:
    
    AwaitingAdultPinLayer* callBackNode;
    
private:
    
    // Login API success
    void onLoginAnswerReceived(const std::string& responseString, const std::string& headerString);
    // Update billing API success
    void onUpdateBillingDataAnswerReceived(const std::string& responseString);
    // Update parent PIN API success
    void onUpdateParentPinAnswerReceived(const std::string& responseString);
    // Register parent API success
    void onRegisterParentAnswerReceived();
    // Get children API success
    void onGetChildrenAnswerReceived(const std::string& responseString);
    // Child login success
    void onChildLoginAnswerReceived(const std::string& responseString, const std::string& headerString);
    // Session cookies success
    void onSessionCookiesAnswerReceived(const std::string& responseString);
    // Register child API success
    void onRegisterChildAnswerReceived();
    // Update child API success
    void onUpdateChildAnswerReceived();
    // Get Force Update Data success
    void onGetForceUpdateDataAnswerReceived(const std::string& responseString);
    
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
    
    // Check for offline mode
    void offlineCheck();
    // Check the clients current ip
    void ipCheck();
    // Login a user
    void login(const std::string& username, const std::string& password);
    // Login a device ID
    void anonymousDeviceLogin();
    // Update billing information from the server
    void updateBillingData();
    // Update parent PIN from the server, calling back to AwaitingAdultPinLayer as part of the flow
    void updateParentPin(AwaitingAdultPinLayer* callBackTo);
    // Register a new parent account
    void registerParent(const std::string& emailAddress, const std::string& password, const std::string& pinNumber, const std::string& marketingAccepted);
    // Get the children linked to the current parent from the backend
    void getAvailableChildren();
    // Login child profile with index
    void childLogin(int childNumber);
    // Register a new child profile on the current parent
    void registerChild(const std::string& childProfileName, const std::string& childGender, const std::string& childDOB, int oomeeNumber);
    // Update a child profile
    void updateChild(const std::string& childId, const std::string& childProfileName, const std::string& childGender, const std::string& childDOB, int oomeeNumber);
    // Update child avatar
    void updateChildAvatar(const std::string& childId, const std::string& imageData);
    // Get sssion cookies
    void getSessionCookies();
    // Verify a google payment
    void verifyGooglePayment(const std::string& orderId, const std::string& iapSku, const std::string& purchaseToken);
    // Verify an Amazon payment
    void verifyAmazonPayment(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserid);
    // Verify an Apple payment
    void verifyApplePayment(const std::string& receiptData, const std::string& transactionID);
    // Get HQ content
    void getHQContent(const std::string& url, const std::string& category);
    // Get Single Content Details
    void GetContent(const std::string& requestId, const std::string& contentID);
    // Reset Password
    void resetPasswordRequest(const std::string& emailAddress);
    // Get force update data
    void getForceUpdateData();
    // Get Parent details
    void getParentDetails();
	// get Child Inventory
	void getChildInventory();
};

NS_AZOOMEE_END

#endif
