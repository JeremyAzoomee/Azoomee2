//
//  IAPFlowController.h
//  azoomee2
//
//  Created by Macauley on 18/01/2018.
//

#ifndef IAPFlowController_h
#define IAPFlowController_h

#include "DynamicNodeFlowController.h"
#include <AzoomeeCommon/UI/RequestAdultPinLayer.h>

NS_AZOOMEE_BEGIN

class IAPFlowController : public DynamicNodeFlowController, public RequestAdultPinLayerDelegate
{
private:
    static const std::string kIAPUpgradeCTAName;
    static const std::string kCoppaPrivacyCTAName;
    static const std::string kLearnMoreCTAName;
    static const std::string kAgeGateCTAName;
    
    const std::string kPathIAP = "iap";
    const std::string kPathRestore = "restore";
    const std::string kPathLearnMore = "learnMore";
    const std::string kPathCoppa = "coppa";
    
    ButtonActionDataRef _actionData = nullptr;
    int _targetVal;
    
    void handleIAPUpgradeFlow(const ButtonActionDataRef& actionData, FlowPath pathAction);
    void handleCoppaPrivacyFlow(const ButtonActionDataRef& actionData, FlowPath pathAction);
    void handleLearnMoreFlow(const ButtonActionDataRef& actionData, FlowPath pathAction);
    void handleAgeGateFlow(const ButtonActionDataRef& actionData, FlowPath pathAction);
    
    void startAgeGate();
    void startIAP();
    
public:
    static DynamicNodeFlowControllerRef create();
    virtual void processAction(const ButtonActionDataRef& actionData) override;
    IAPFlowController() noexcept;

    // delegate functions
    void AdultPinCancelled(RequestAdultPinLayer* layer) override;
    void AdultPinAccepted(RequestAdultPinLayer* layer) override;
};

NS_AZOOMEE_END

#endif /* IAPFlowController_h */
