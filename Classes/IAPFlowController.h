//
//  IAPFlowController.h
//  azoomee2
//
//  Created by Macauley on 18/01/2018.
//

#ifndef IAPFlowController_h
#define IAPFlowController_h

#include "DynamicNodeFlowController.h"
#include "AwaitingAdultPinLayer.h"

NS_AZOOMEE_BEGIN

class IAPFlowController : public DynamicNodeFlowController, public AwaitingAdultPinLayerDelegate
{
private:
    static const std::string kIAPUpgradeCTAName;
    static const std::string kCoppaPrivacyCTAName;
    static const std::string kLearnMoreCTAName;
    
    const std::string kPathIAP = "iap";
    const std::string kPathRestore = "restore";
    const std::string kPathLearnMore = "learnMore";
    const std::string kPathCoppa = "coppa";
    
    std::string _contextExtention = "default";
    
    ButtonActionDataRef _actionData = nullptr;
    
    void handleIAPUpgradeFlow(const ButtonActionDataRef& actionData, FlowPath pathAction);
    void handleCoppaPrivacyFlow(const ButtonActionDataRef& actionData, FlowPath pathAction);
    void handleLearnMoreFlow(const ButtonActionDataRef& actionData, FlowPath pathAction);
    
    void startIAP();
    
public:
    static DynamicNodeFlowControllerRef createWithContext(const std::string& context);
    static DynamicNodeFlowControllerRef create();
    virtual void processAction(const ButtonActionDataRef& actionData) override;
    IAPFlowController(const std::string& context = "default") noexcept;

    // delegate functions
    void AdultPinCancelled(AwaitingAdultPinLayer* layer) override;
    void AdultPinAccepted(AwaitingAdultPinLayer* layer) override;
};

NS_AZOOMEE_END

#endif /* IAPFlowController_h */
